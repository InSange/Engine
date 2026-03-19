#pragma once
#include "GameObject/NGameObject.h"

namespace NuNu
{
	class Demon : public GameObject
	{
	public:
		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

	private:
	};
}


