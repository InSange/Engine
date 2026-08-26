#pragma once
#include "guiEditorWindow.h"
#include "guiEditor.h"

#include "GameObject/NGameObject.h"
#include "Component/Camera/NEditorCamera.h"

namespace gui
{
	class SceneWindow : public EditorWindow
	{
	public:
		SceneWindow();
		~SceneWindow();

		void Initialize() override;
		void Update() override;
		void OnGUI() override;
		void Run() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDestroy() override;

		void SetGuizmoType(int type) { GuizmoType = type; }

	private:
		std::vector<Editor*> mEditors;
		NuNu::GameObject* mEditorCameraObject;
		NuNu::EditorCamera* mEditorCamera;

		NuNu::math::Vector2 ViewportBounds[2];
		NuNu::math::Vector2 ViewportSize;
		bool ViewportFocused;
		bool ViewportHovered;
		int GuizmoType;
	};
}