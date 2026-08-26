#include "guiHierarchyWindow.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "Scene/NSceneManager.h"
#include "Scene/NScene.h"
#include "Layer/NLayer.h"
#include "GameObject/NGameObject.h"
#include "Object/NObject.h"

namespace gui
{
	static const char* kLayerNames[] =
	{
		"None", "BackGround", "Player", "Floor", "Particle",
		"Layer5","Layer6","Layer7","Layer8","Layer9",
		"Layer10","Layer11","Layer12","Layer13","Layer14","Layer15",
	};

	static std::string toStr(const std::wstring& ws)
	{
		if (ws.empty()) return "";
		int n = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string s(n - 1, 0);
		WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, s.data(), n, nullptr, nullptr);
		return s;
	}

	HierarchyWindow::HierarchyWindow()
	{
		SetName("Hierarchy");
		SetSize(ImVec2(300, 600));
	}

	HierarchyWindow::~HierarchyWindow() {}

	void HierarchyWindow::Initialize() {}
	void HierarchyWindow::Update()     {}

	void HierarchyWindow::OnGUI()
	{
		NuNu::Scene* scene = NuNu::SceneManager::GetActiveScene();
		if (scene == nullptr)
		{
			ImGui::TextDisabled("No active scene");
			return;
		}

		std::string sceneName = toStr(scene->GetName());
		ImGui::Text("Scene: %s", sceneName.c_str());
		ImGui::Separator();

		// ── 빈 공간 우클릭 → 오브젝트 생성 ────────────────────────────────────
		if (ImGui::BeginPopupContextWindow("##hierarchy_ctx",
			ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Empty GameObject"))
				{
					auto* obj = NuNu::object::Instantiate<NuNu::GameObject>(NuNu::eLayerType::None);
					obj->SetName(L"GameObject");
					NuNu::renderer::selectedObject = obj;
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		const auto& layers = scene->GetLayers();
		int uid = 0;

		for (int li = 0; li < (int)layers.size(); li++)
		{
			auto& objs = layers[li]->GetGameObjects();
			if (objs.empty()) continue;

			const char* layerLabel = kLayerNames[li];
			if (ImGui::CollapsingHeader(layerLabel, ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (NuNu::GameObject* obj : objs)
				{
					if (obj == nullptr) continue;

					std::string name = toStr(obj->GetName());
					if (name.empty()) name = "(unnamed)";

					std::string label = name + "##obj" + std::to_string(uid++);
					bool selected = (NuNu::renderer::selectedObject == obj);

					if (selected)
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.3f, 0.9f, 1.0f, 1.0f });

					if (ImGui::Selectable(label.c_str(), selected))
						NuNu::renderer::selectedObject = obj;

					if (selected)
						ImGui::PopStyleColor();

					// 선택된 오브젝트 우클릭 컨텍스트
					if (selected && ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete"))
						{
							NuNu::object::Destroy(obj);
							NuNu::renderer::selectedObject = nullptr;
						}
						if (ImGui::MenuItem("Duplicate"))
						{
							auto* dup = NuNu::object::Instantiate<NuNu::GameObject>(obj->GetLayerType());
							dup->SetName(obj->GetName() + L"_Copy");
							auto* srcTr = obj->GetComponent<NuNu::Transform>();
							auto* dstTr = dup->GetComponent<NuNu::Transform>();
							if (srcTr && dstTr)
							{
								dstTr->SetPosition(srcTr->GetPosition());
								dstTr->SetRotation(srcTr->GetRotation());
								dstTr->SetScale(srcTr->GetScale());
							}
							NuNu::renderer::selectedObject = dup;
						}
						ImGui::EndPopup();
					}
				}
			}
		}

		// Delete 키로 선택 오브젝트 삭제
		if (NuNu::renderer::selectedObject && ImGui::IsWindowFocused()
			&& ImGui::IsKeyPressed(ImGuiKey_Delete))
		{
			NuNu::object::Destroy(NuNu::renderer::selectedObject);
			NuNu::renderer::selectedObject = nullptr;
		}
	}

	void HierarchyWindow::Run()
	{
		bool Active = (bool)GetState();
		ImGui::Begin(GetName().c_str(), &Active, GetFlag());
		Update();
		OnGUI();
		ImGui::End();
	}

	void HierarchyWindow::OnEnable()  {}
	void HierarchyWindow::OnDisable() {}
	void HierarchyWindow::OnDestroy() {}
}
