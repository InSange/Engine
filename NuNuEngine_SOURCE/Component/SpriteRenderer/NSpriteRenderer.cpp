#include "Component/SpriteRenderer/NSpriteRenderer.h"
#include "Component/Transform/NTransform.h"
#include "GameObject/NGameObject.h"
#include "Renderer/NRenderer.h"
namespace NuNu
{
	SpriteRenderer::SpriteRenderer()
		: Component(enums::eComponentType::SpriteRenderer)
		, mTexture(nullptr)
		, mSize(Vector2::One)
	{
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}

	void SpriteRenderer::Initialize()
	{
	}

	void SpriteRenderer::Update()
	{
	}

	void SpriteRenderer::LateUpdate()
	{
	}

	void SpriteRenderer::Render()
	{
		if (mTexture == nullptr)
			assert(false);


	/*	Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
		float rot = tr->GetRotation();
		Vector2 scale = tr->GetScale();*/

/*		pos = renderer::mainCamera->CalculatePosition(pos);

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
					, mTexture->GetWidth() * mSize.x * scale.x, mTexture->GetHeight() * mSize.y * scale.y
					, mTexture->GetHdc(), 0, 0, mTexture->GetWidth(), mTexture->GetHeight(), func);
			}
			else
			{
				TransparentBlt(hdc
					, pos.x, pos.y
					, mTexture->GetWidth() * mSize.x * scale.x, mTexture->GetHeight() * mSize.y * scale.y
					, mTexture->GetHdc(), 0, 0, mTexture->GetWidth(), mTexture->GetHeight(), RGB(255, 0, 255));
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
				static_cast<INT>(mTexture->GetWidth() * mSize.x * scale.x),
				static_cast<INT>(mTexture->GetHeight() * mSize.y * scale.y)
			);

			graphics.DrawImage(mTexture->GetImage(),
				destRect,
				0, 0,
				static_cast<INT>(mTexture->GetWidth()),
				static_cast<INT>(mTexture->GetHeight()),
				Gdiplus::UnitPixel, nullptr);//&imgAtt);
		}*/
	}
}

