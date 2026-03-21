#pragma once
#include "Component/Component/NComponent.h"

namespace NuNu
{
	class AudioListener : public Component
	{
	public:
		AudioListener();
		~AudioListener();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;
	};
}


