#include "NTilemapRenderer.h"
#include "NTransform.h"
#include "NGameObject.h"
#include "NRenderer.h"
#include "NApplication.h"

namespace NuNu
{
	Vector2 TilemapRenderer::TileSize = Vector2::One;
	Vector2 TilemapRenderer::OriginTileSize = Vector2::One;
	Vector2 TilemapRenderer::SelectedIndex = Vector2(-1.0f, -1.0f);

	TilemapRenderer::TilemapRenderer()
		: Component(enums::eComponentType::SpriteRenderer)
		, mTexture(nullptr)
		, mSize(Vector2::One)
		, mIndex(Vector2::Zero)
		, mTileSize(16.0f, 16.0f)
	{
		TileSize = mTileSize * mSize;
		OriginTileSize = mTileSize;
	}

	TilemapRenderer::~TilemapRenderer()
	{
	}

	void TilemapRenderer::Initialize()
	{
	}

	void TilemapRenderer::Update()
	{
	}

	void TilemapRenderer::LateUpdate()
	{
	}

	void TilemapRenderer::Render(HDC hdc)
	{
		if (mTexture == nullptr)
			assert(false);


		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
		float rot = tr->GetRotation();
		Vector2 scale = tr->GetScale();

		pos = renderer::mainCamera->CalculatePosition(pos);

		float finalWidth = mTileSize.x * mSize.x * scale.x;
		float finalHeight = mTileSize.y * mSize.y * scale.y;

		extern NuNu::Application application;
		float screenWidth = application.GetWidth();
		float screenHeight = application.GetHeight();

		if (pos.x + finalWidth < 0.0f || pos.x > screenWidth ||
			pos.y + finalHeight < 0.0f || pos.y > screenHeight)
		{
			// 화면 밖이므로, 그리지 않고 바로 함수를 종료합니다! 
			return;
		}

		if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Bmp)
		{
			if (mTexture->IsAlpha())
			{
				BLENDFUNCTION func = {};
				func.BlendOp = AC_SRC_OVER;
				func.BlendFlags = 0;
				func.AlphaFormat = AC_SRC_ALPHA;
				func.SourceConstantAlpha = 255;

				AlphaBlend(hdc
					, pos.x, pos.y
					, mTileSize.x * mSize.x * scale.x, mTileSize.y * mSize.y * scale.y
					, mTexture->GetHdc(), mIndex.x * mTileSize.x, mIndex.y * mTileSize.y, mTileSize.x, mTileSize.y, func);
			}
			else
			{
				TransparentBlt(hdc
					, pos.x, pos.y
					, mTileSize.x * mSize.x * scale.x, mTileSize.y * mSize.y * scale.y
					, mTexture->GetHdc(), mIndex.x * mTileSize.x, mIndex.y * mTileSize.y, mTileSize.x, mTileSize.y, RGB(255, 0, 255));
			}
		}
		else if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Png || mTexture->GetTextureType() == graphics::Texture::eTextureType::jpg)
		{
			Gdiplus::ImageAttributes imgAtt = {}; // 투명화 추가
			imgAtt.SetColorKey(Gdiplus::Color(200, 200, 200), Gdiplus::Color(255, 255, 255));

			Gdiplus::Graphics graphics(hdc);

			graphics.TranslateTransform(pos.x, pos.y);
			graphics.RotateTransform(rot);
			graphics.TranslateTransform(-pos.x, -pos.y);

			Gdiplus::Rect destRect(
				static_cast<INT>(pos.x),
				static_cast<INT>(pos.y),
				static_cast<INT>(mTileSize.x * mSize.x * scale.x),
				static_cast<INT>(mTileSize.y * mSize.y * scale.y)
			);

			graphics.DrawImage(mTexture->GetImage(),
				destRect,
				mIndex.x * mTileSize.x, mIndex.y * mTileSize.y,
				static_cast<INT>(mTileSize.x),
				static_cast<INT>(mTileSize.y),
				Gdiplus::UnitPixel, nullptr);//&imgAtt);
		}
	}
}
