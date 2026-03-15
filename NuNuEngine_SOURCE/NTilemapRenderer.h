#pragma once
#include "NEntity.h"
#include "NComponent.h"
#include "NTexture.h"

namespace NuNu
{
	class TilemapRenderer : public Component
	{
	public:
		TilemapRenderer();
		~TilemapRenderer();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetTexture(graphics::Texture* tex) { mTexture = tex; }
		void SetSize(math::Vector2 size) { mSize = size; }
		Vector2 GetIndex() { return mIndex; }
		void SetIndex(Vector2 index) { mIndex = index; }

	public:
		static Vector2 TileSize;
		static Vector2 OriginTileSize;
		static Vector2 SelectedIndex;

	private:
		Vector2 mTileSize;

		graphics::Texture* mTexture;
		Vector2 mSize;
		Vector2 mIndex;
	};
}

