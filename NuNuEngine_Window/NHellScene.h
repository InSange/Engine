#pragma once
#include "../NuNuEngine_SOURCE/NScene.h"

namespace NuNu
{
	class HellScene : public Scene
	{
	public:
		HellScene();
		~HellScene();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
	private:
	};
}

