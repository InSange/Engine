#pragma once
#include "NComponent.h"
#include "NTexture.h"

namespace NuNu
{
	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetTexture(graphics::Texture* tex) { mTexture = tex; }
		void SetSize(math::Vector2 size) { mSize = size; }

	private:
		graphics::Texture* mTexture;
		math::Vector2 mSize;
	};
}

