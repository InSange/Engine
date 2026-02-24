#pragma once
#include "../NuNuEngine_SOURCE/NGameObject.h"

namespace NuNu
{
	class Player : public GameObject
	{
	public:
		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

	private:
	};
}

