#pragma once
#include "Component/Collider/NCollider.h"

namespace NuNu
{
	class BoxCollider2D : public Collider
	{
	public:
		BoxCollider2D();
		~BoxCollider2D();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	private:
		Vector2 mSize;
	};
}


