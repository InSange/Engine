#pragma once
#include "Component/Component/NComponent.h"

namespace NuNu
{
	class AudioListener final : public Component
	{
	public:
		AudioListener();
		virtual ~AudioListener();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(const Matrix& view, const Matrix& projection) override;
	};
}


