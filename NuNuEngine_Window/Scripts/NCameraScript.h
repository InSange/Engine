#pragma once
#include "Component/Script/NScript.h"
#include "Component/Transform/NTransform.h"

namespace NuNu
{
	class CameraScript : public Script
	{
	public:
		CameraScript();
		~CameraScript();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;
	private:
	};
}


