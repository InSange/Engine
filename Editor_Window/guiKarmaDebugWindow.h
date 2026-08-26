#pragma once
#include "guiEditorWindow.h"

namespace gui
{
	class KarmaDebugWindow : public EditorWindow
	{
	public:
		KarmaDebugWindow();
		virtual ~KarmaDebugWindow() = default;

		void Initialize() override;
		void Update()     override;
		void OnGUI()      override;
		void Run()        override;
		void OnEnable()   override {}
		void OnDisable()  override {}
		void OnDestroy()  override {}
	};
}
