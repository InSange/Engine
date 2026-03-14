#pragma once
#include "NCollider.h"

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
		virtual void Render(HDC hdc);
	private:
		float mRadius;
	};
}

