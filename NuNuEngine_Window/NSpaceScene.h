#pragma once
#include "../NuNuEngine_SOURCE/NScene.h"

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
		void Render(HDC hdc) override;
	private:

	};
}

