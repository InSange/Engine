#include "guiHierarchyWindow.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "Scene/NSceneManager.h"
#include "Scene/NScene.h"
#include "Layer/NLayer.h"
#include "GameObject/NGameObject.h"

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
				}
			}
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
