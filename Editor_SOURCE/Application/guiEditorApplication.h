#pragma once

#include "../GUI/Editor/guiEditor.h"
#include "../GUI/EditorWindow/guiEditorWindow.h"
#include "../Application/guiImguiEditor.h"

#include "../../NuNuEngine_SOURCE/Graphics/RenderTarget/NRenderTarget.h"
#include "../../NuNuEngine_SOURCE/Event/NEvent.h"
#include "../../NuNuEngine_SOURCE/Event/NKeyEvent.h"
#include "../../NuNuEngine_SOURCE/Event/NMouseEvent.h"
#include "../../NuNuEngine_SOURCE/Component/Camera/NEditorCamera.h"

namespace NuNu
{
	class KeyPressedEvent;
}

namespace gui
{
	/// <summary>
	/// EditorApplication 클래스는 에디터에서 사용되는 메인 클래스로
	/// 에디터의 메인 루프를 실행하고 에디터의 초기화 및 종료를 담당합니다.
	/// </summary>
	class EditorApplication
	{
	public:
		enum class eState
		{
			Disable,
			Active,
			Destroy,
		};

		template <typename T>
		T* GetWindow(const std::wstring& name)
		{
			auto iter = mEditorWindows.find(name);
			if (iter == mEditorWindows.end())
				return nullptr;

			return dynamic_cast<T*>(iter->second);
		}

		///<summary>
		///에디터를 초기화합니다.
		///</summary>
		static bool Initialize();

		/// <summary>
		/// 에디터를 로직을 업데이트합니다.
		/// </summary>
		static void Update();

		/// <summary>
		/// 에디터 렌더링을 업데이트합니다.
		/// </summary>
		static void OnGUI();

		/// <summary>
		/// 에디터 업데이트
		/// </summary>
		static void Run();

		/// <summary>
		/// 에디터를 종료합니다.
		/// </summary>
		static void Release();

		/// <summary>
		/// EditorApplication 이벤트 처리
		/// </summary>
		static void OnEvent(NuNu::Event& e);

		static void OpenProject();
		static void NewScene();
		static void SaveScene();
		static void SaveSceneAs();
		static void OpenScene(const std::filesystem::path& path);

		static void OnImGuiRender();

		//Event
		static void SetKeyPressed(int keyCode, int scancode, int action, int mods);
		static void SetCursorPos(double x, double y);
		static bool OnKeyPressed(NuNu::KeyPressedEvent& e);

		static void SetGuizmoType(int type) { GuizmoType = type; }

	private:
		static ImguiEditor* ImguiEditor;

		static std::map<std::wstring, EditorWindow*> mEditorWindows;
		static ImGuiWindowFlags Flag;
		static ImGuiDockNodeFlags DockspaceFlags;
		static eState State;
		static bool FullScreen;
		static NuNu::math::Vector2 ViewportBounds[2];
		static NuNu::math::Vector2 ViewportSize;
		static bool ViewportFocused;
		static bool ViewportHovered;
		static int GuizmoType;
		static NuNu::EditorCamera* EditorCamera;

		static NuNu::graphics::RenderTarget* FrameBuffer;
		static NuNu::EventCallbackFn EventCallback;
	};
}
