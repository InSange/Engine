#pragma once
#include "..\\NuNuEngine_SOURCE\\NGameObject.h"

namespace NuNu
{
	class Floor : public GameObject
	{
	public:
		Floor();
		~Floor();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

	private:

	};
}

