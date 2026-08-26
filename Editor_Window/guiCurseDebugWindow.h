#pragma once
#include "guiEditorWindow.h"

namespace gui
{
	class CurseDebugWindow : public EditorWindow
	{
	public:
		CurseDebugWindow();
		virtual ~CurseDebugWindow() = default;

		void Initialize() override;
		void Update()     override;
		void OnGUI()      override;
		void Run()        override;
		void OnEnable()   override {}
		void OnDisable()  override {}
		void OnDestroy()  override {}
	};
}
