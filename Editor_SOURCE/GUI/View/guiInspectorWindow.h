#pragma once
#include "GUI/EditorWindow/guiEditorWindow.h"
#include "GUI/Editor/guiEditor.h"

namespace gui
{
	class InspectorWindow : public EditorWindow
	{
	public:
		InspectorWindow();
		~InspectorWindow();

		void Initialize() override;
		void Update() override;
		void OnGUI() override;
		void Run() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDestroy() override;

	private:
		std::vector<Editor*> mEditors;
	};
}