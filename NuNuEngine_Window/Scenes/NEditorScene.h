#pragma once
#pragma once
#include "../../NuNuEngine_SOURCE/Scene/NScene.h"

namespace NuNu
{
	class EditorScene : public Scene
	{
	public:
		EditorScene();
		virtual ~EditorScene();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

		void OnEnter() override;
		void OnExit() override;

	private:
	};
}