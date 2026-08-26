#pragma once
#include "../EditorWindow/guiEditorWindow.h"
#include "../Editor/guiEditor.h"

namespace gui
{
	class ProjectWindow : public EditorWindow
	{
	public:
		ProjectWindow();
		~ProjectWindow();

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