#pragma once
#include "guiEditorWindow.h"

namespace gui
{
	class StageDebugWindow : public EditorWindow
	{
	public:
		StageDebugWindow();
		virtual ~StageDebugWindow() = default;

		void Initialize() override;
		void Update()     override;
		void OnGUI()      override;
		void Run()        override;
		void OnEnable()   override {}
		void OnDisable()  override {}
		void OnDestroy()  override {}
	};
}
