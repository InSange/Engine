#pragma once
#include "Component/Collider/NCollider.h"

namespace NuNu
{
	class CircleCollider2D : public Collider
	{
	public:
		CircleCollider2D();
		~CircleCollider2D();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	private:
		float mRadius;
	};
}


