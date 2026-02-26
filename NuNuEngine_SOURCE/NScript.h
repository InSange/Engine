#pragma once
#include "NComponent.h"

namespace NuNu
{
	class Script : public Component
	{
	public:
		Script();
		~Script();
		
		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

	private:

	};
}

