#pragma once
#include "Scene/NScene.h"

namespace NuNu
{
	class HellScene : public Scene
	{
	public:
		HellScene();
		virtual ~HellScene();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

		void OnEnter() override;
		void OnExit() override;
	private:
	};
}


