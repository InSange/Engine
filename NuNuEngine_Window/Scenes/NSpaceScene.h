#pragma once
#include "Scene/NScene.h"

namespace NuNu
{
	class SpaceScene : public Scene
	{
	public:
		SpaceScene();
		~SpaceScene();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

		void OnEnter() override;
		void OnExit() override;
	private:

	};
}


