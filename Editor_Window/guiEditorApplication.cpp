#include "guiEditorApplication.h"
#include "guiInspectorWindow.h"
#include "guiConsoleWindow.h"
#include "guiProjectWindow.h"
#include "guiSceneWindow.h"
#include "guiHierarchyWindow.h"
#include "guiCurseDebugWindow.h"

#include "High Level Interface/NApplication.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "GameObject/NGameObject.h"
#include "Component/Transform/NTransform.h"
#include "Helpers/NInput.h"
#include "Helpers/NTime.h"
#include "Event/NMouseEvent.h"

extern NuNu::Application application;

namespace gui
{
	ImguiEditor* EditorApplication::ImguiEditor = nullptr;
	std::map<std::wstring, EditorWindow*> EditorApplication::EditorWindows;
	ImGuiWindowFlags EditorApplication::Flag = ImGuiWindowFlags_None;
	ImGuiDockNodeFlags EditorApplication::DockspaceFlags = ImGuiDockNodeFlags_None;
	EditorApplication::eState EditorApplication::State = EditorApplication::eState::Active;
	bool EditorApplication::FullScreen = true;
	//NuNu::math::Vector2 EditorApplication::ViewportBounds[2] = {};
	NuNu::math::Vector2 EditorApplication::ViewportSize;
	bool EditorApplication::ViewportFocused = false;
	bool EditorApplication::ViewportHovered = false;
	int EditorApplication::GuizmoType = -1;

	NuNu::graphics::RenderTarget* EditorApplication::FrameBuffer = nullptr;
	NuNu::EventCallbackFn EditorApplication::EventCallback = nullptr;

	bool EditorApplication::Initialize()
	{
#ifdef _DEBUG
		if (::AllocConsole() == TRUE)
		{
			FILE* nfp[3];
			freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
			freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
			freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
			std::ios::sync_with_stdio();
		}

		std::cout << "Console Open" << std::endl;
#endif

		ImguiEditor = new gui::ImguiEditor();
		FrameBuffer = NuNu::renderer::FrameBuffer;

		ImguiEditor->Initialize();

		//HierarchyWindow
		HierarchyWindow* hierarchy = new HierarchyWindow();
		EditorWindows.insert(std::make_pair(L"HierarchyWindow", hierarchy));
		
		// InspectorWindow
		InspectorWindow* inspector = new InspectorWindow();
		EditorWindows.insert(std::make_pair(L"InspectorWindow", inspector));
		EventCallback = &EditorApplication::OnEvent;

		//ProjectWindow
		ProjectWindow* project = new ProjectWindow();
		EditorWindows.insert(std::make_pair(L"ProjectWindow", project));

		//SceneWindow
		SceneWindow* scene = new SceneWindow();
		EditorWindows.insert(std::make_pair(L"SceneWindow", scene));

		//CosoleWindow
		ConsoleWindow* console = new ConsoleWindow();
		EditorWindows.insert(std::make_pair(L"ConsoleWindow", console));

		// CurseDebugWindow
		CurseDebugWindow* curseDebug = new CurseDebugWindow();
		EditorWindows.insert(std::make_pair(L"CurseDebugWindow", curseDebug));

		return true;
	}

	void EditorApplication::Update()
	{
	}

	void EditorApplication::OnGUI()
	{
		ImguiEditor->Begin();
		OnImGuiRender();
		ImguiEditor->End();
	}

	void EditorApplication::Run()
	{
		Update();
		OnGUI();
	}

	void EditorApplication::UpdatePlatformWindows()
	{
		ImguiEditor->UpdatePlatformWindows();
	}

	void EditorApplication::Release()
	{
		for (auto iter : EditorWindows)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		// Cleanup
		delete ImguiEditor;
		ImguiEditor = nullptr;

		// Release Console
#ifdef _DEBUG
		FreeConsole();
#endif
	}

	void EditorApplication::OnEvent(NuNu::Event& e)
	{
		NuNu::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<NuNu::KeyPressedEvent>([](NuNu::KeyPressedEvent& e) -> bool
			{
				// Todo : KeyPressedEvent
				if (OnKeyPressed(e))
					return true;

				return false;
			});

		dispatcher.Dispatch<NuNu::KeyReleasedEvent>([](NuNu::KeyReleasedEvent& e) -> bool
			{
				// Todo : KeyReleasedEvent
				//if (OnKeyPressed(e))
					//return true;

				return false;
			});

		dispatcher.Dispatch<NuNu::MouseMovedEvent>([](NuNu::MouseMovedEvent& e) -> bool
			{
				// Todo : MouseMovedEvent

				return true;
			});

		if (!e.Handled)
		{
			ImguiEditor->OnEvent(e);
		}
	}

	void EditorApplication::SetupInitialDockLayout()
	{
		// 크기 변경 감지 + debounce: 2프레임 안정 후 재빌드
		static ImVec2 sPending   = { -1.0f, -1.0f };
		static ImVec2 sLastBuilt = { -2.0f, -2.0f };
		static int    sStable    = 0;

		ImVec2 size = ImGui::GetMainViewport()->WorkSize; // 타스크바 제외 작업 영역

		if (fabsf(sPending.x - size.x) >= 1.0f || fabsf(sPending.y - size.y) >= 1.0f)
		{
			sPending = size;
			sStable  = 0;
			return; // 크기 변화 중 — 안정될 때까지 대기
		}
		if (++sStable < 2) return; // 2프레임 안정 대기

		if (fabsf(sLastBuilt.x - size.x) < 1.0f && fabsf(sLastBuilt.y - size.y) < 1.0f)
			return; // 이미 이 크기로 빌드됨
		sLastBuilt = size;

		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, size);

		// ┌──────────────────────────────┬───────────┐
		// │  Scene (좌)  │  Game (우)    │ Hierarchy │
		// │              │               ├───────────┤
		// │              │               │ Inspector │
		// ├──────────────┴───────────────┴───────────┤
		// │        Project / Console (탭)             │
		// └───────────────────────────────────────────┘

		// 1. 하단: Project/Console — 전체 너비 25%
		ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);

		// 2. 우측: Hierarchy + Inspector — 20%
		ImGuiID dock_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.20f, nullptr, &dockspace_id);

		// 3. 우측 패널 수직 분할: 상단 Hierarchy / 하단 Inspector
		ImGuiID dock_inspector = ImGui::DockBuilderSplitNode(dock_right, ImGuiDir_Down, 0.55f, nullptr, &dock_right);

		// 4. 중앙 뷰 좌우 분할: Scene (좌) / Game (우)
		ImGuiID dock_game = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.50f, nullptr, &dockspace_id);

		// 5. 도킹
		ImGui::DockBuilderDockWindow("Scene",          dockspace_id);
		ImGui::DockBuilderDockWindow("Game",           dock_game);
		ImGui::DockBuilderDockWindow("Hierarchy",      dock_right);
		ImGui::DockBuilderDockWindow("Inspector",      dock_inspector);
		ImGui::DockBuilderDockWindow("Curse Debugger", dock_inspector);
		ImGui::DockBuilderDockWindow("Project",        dock_bottom);
		ImGui::DockBuilderDockWindow("Console",        dock_bottom);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	void EditorApplication::OpenProject()
	{
	}

	void EditorApplication::NewScene()
	{
	}

	void EditorApplication::SaveScene()
	{
	}

	void EditorApplication::SaveSceneAs()
	{
	}

	void EditorApplication::OpenScene(const std::filesystem::path& path)
	{

	}

	void EditorApplication::OnImGuiRender()
	{
		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);

		// Our state
/*		bool show_demo_window = true;
		bool show_another_window = false;*/
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
				// because it would be confusing to have two docking targets within each others.
		Flag = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (FullScreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			Flag |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			Flag |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (DockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			Flag |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		bool Active = static_cast<bool>(State);
		ImGui::Begin("EditorApplication", &Active, Flag);
		ImGui::PopStyleVar();

		if (FullScreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), DockspaceFlags);

			SetupInitialDockLayout();
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
					OpenProject();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
					application.Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
				{
					//ScriptEngine::ReloadAssembly(); 추후 C#스크립트 추가기능이 생기면 추가할 예정
				}

				ImGui::EndMenu();
			}


			ImGui::EndMenuBar();
		}

		auto iter = EditorWindows.find(L"SceneWindow");
		dynamic_cast<SceneWindow*>(iter->second)->SetGuizmoType(GuizmoType);

		// check if the mouse,keyboard is on the Sceneview
		ViewportFocused = ImGui::IsWindowFocused();
		ViewportHovered = ImGui::IsWindowHovered();

		// to do : mouse, keyboard event
		ImguiEditor->BlockEvent(!ViewportHovered);

		// viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Game");

		// rendering game view from SceneCamera
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ViewportSize = Vector2{ viewportPanelSize.x, viewportPanelSize.y };

		D3D12_GPU_DESCRIPTOR_HANDLE gameSRV = NuNu::graphics::GetDevice()->GetGameSRVGpuHandle();
		if (gameSRV.ptr != 0)
			ImGui::Image((ImTextureID)gameSRV.ptr, ImVec2{ ViewportSize.x, ViewportSize.y },
				ImVec2{ 0, 0 }, ImVec2{ 1, 1 });

		// HUD overlay: FPS + player position + forward
		{
			float dt = NuNu::Time::DeltaTime();
			float fps = (dt > 0.0f) ? (1.0f / dt) : 0.0f;

			constexpr float x    = 10.0f;
			constexpr float topY = 24.0f;
			constexpr float lineH = 18.0f;

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f });

			ImGui::SetCursorPos(ImVec2{ x, topY });
			ImGui::Text("FPS: %.1f", fps);

			if (NuNu::renderer::mainCamera != nullptr)
			{
				NuNu::Transform* tr = NuNu::renderer::mainCamera->GetOwner()->GetComponent<NuNu::Transform>();
				NuNu::math::Vector3 pos = tr->GetPosition();
				NuNu::math::Vector3 fwd = tr->Forward();

				ImGui::SetCursorPos(ImVec2{ x, topY + lineH });
				ImGui::Text("Pos: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);

				ImGui::SetCursorPos(ImVec2{ x, topY + lineH * 2.0f });
				ImGui::Text("Fwd: %.2f, %.2f, %.2f", fwd.x, fwd.y, fwd.z);
			}

			ImGui::PopStyleColor();
		}

		// Open Scene by drag and drop
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PROJECT_ITEM"))
			{
				const auto path = static_cast<const wchar_t*>(payload->Data);
				OpenScene(path);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End(); // Game end
		ImGui::PopStyleVar();

		// EditorWindows는 Game 윈도우 밖에서 실행해야 독립 도킹 노드로 처리됨
		for (auto& iter : EditorWindows)
			iter.second->Run();

		ImGui::End(); // dockspace end
	}

	// Events
	void EditorApplication::SetKeyPressed(int keyCode, int scancode, int action, int mods)
	{
		constexpr int RELEASE = 0;
		constexpr int PRESS = 1;
		constexpr int REPEAT = 2;

		switch (action)
		{
		case RELEASE:
		{
			NuNu::KeyReleasedEvent event(static_cast<NuNu::eKeyCode>(keyCode));

			if (EventCallback)
				EventCallback(event);
		}
		break;
		case PRESS:
		{
			NuNu::KeyPressedEvent event(static_cast<NuNu::eKeyCode>(keyCode), false);

			if (EventCallback)
				EventCallback(event);
		}
		break;
		case REPEAT:
		{
			NuNu::KeyPressedEvent event(static_cast<NuNu::eKeyCode>(keyCode), true);

			if (EventCallback)
				EventCallback(event);
		}
		break;
		}
	}

	void EditorApplication::SetCursorPos(double x, double y)
	{
		NuNu::MouseMovedEvent event(x, y);

		if (EventCallback)
			EventCallback(event);
	}

	bool EditorApplication::OnKeyPressed(NuNu::KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		bool control = NuNu::Input::GetKey(NuNu::eKeyCode::Leftcontrol) || NuNu::Input::GetKey(NuNu::eKeyCode::RightControl);
		bool shift = NuNu::Input::GetKey(NuNu::eKeyCode::LeftShift) || NuNu::Input::GetKey(NuNu::eKeyCode::RightShift);

		switch (e.GetKeyCode())
		{
			// Gizmos
		case NuNu::eKeyCode::Q:
		{
			if (!ImGuizmo::IsUsing())
				SetGuizmoType(-1);
			break;
		}
		case NuNu::eKeyCode::W:
		{
			if (!ImGuizmo::IsUsing())
				SetGuizmoType(ImGuizmo::OPERATION::TRANSLATE);
			break;
		}
		case NuNu::eKeyCode::E:
		{
			if (!ImGuizmo::IsUsing())
				SetGuizmoType(ImGuizmo::OPERATION::ROTATE);
			break;
		}
		case NuNu::eKeyCode::R:
		{
			if (control)
			{
				//ScriptEngine::ReloadAssembly();
			}
			else
			{
				if (!ImGuizmo::IsUsing())
					SetGuizmoType(ImGuizmo::OPERATION::SCALE);
			}
			break;
		}
		}

		return true;
	}
}
