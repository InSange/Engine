#pragma once
#include "../../NuNuEngine_SOURCE/GameObject/NGameObject.h"

namespace NuNu
{
	class ProjectTile : public GameObject
	{
	public:
		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;
	};
}