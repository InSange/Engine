#include "Component/Animation/NAnimation.h"
#include "Helpers/NTime.h"
#include "Component/Transform/NTransform.h"
#include "GameObject/NGameObject.h"
#include "Component/Animation/NAnimator.h"
#include "Renderer/NRenderer.h"

namespace NuNu
{
	Animation::Animation()
		: Resource(enums::eResourceType::Animation)
		, mAnimator(nullptr)
		, mTexture(nullptr)
		, mAnimationSheet{}
		, mIndex(-1)
		, mTime(0.0f)
		, mbComplete(false)
	{
	}
	Animation::~Animation()
	{
	}
	HRESULT Animation::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	void Animation::Update()
	{
		if (mbComplete) return;

		mTime += Time::DeltaTime();

		if (mAnimationSheet[mIndex].duration < mTime)
		{
			mTime = 0.0f;
			if (mIndex < mAnimationSheet.size() - 1)
			{
				mIndex++;
			}
			else
			{
				mbComplete = true;
			}
		}
	}
	void Animation::Render(HDC hdc)
	{
		if (mTexture == nullptr) return;

		GameObject* gameObj = mAnimator->GetOwner();
		Transform* tr = gameObj->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();
		float rot = tr->GetRotation();
		Vector2 scale = tr->GetScale();

		if (renderer::mainCamera) pos = renderer::mainCamera->CalculatePosition(pos);

		Sprite sprite = mAnimationSheet[mIndex];

		float finalWidth = sprite.size.x * scale.x;
		float finalHeight = sprite.size.y * scale.y;

		float drawX = pos.x - (finalWidth / 2.0f) + sprite.offset.x;
		float drawY = pos.y - (finalHeight / 2.0f) + sprite.offset.y;

		// BMP
		if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Bmp)
		{
			HDC imgHdc = mTexture->GetHdc();

			if (mTexture->IsAlpha())
			{
				BLENDFUNCTION func = {};
				func.BlendOp = AC_SRC_OVER;
				func.BlendFlags = 0;
				func.AlphaFormat = AC_SRC_ALPHA;
				func.SourceConstantAlpha = 255;

				AlphaBlend(hdc
					, static_cast<int>(drawX), static_cast<int>(drawY)
					, static_cast<int>(finalWidth), static_cast<int>(finalHeight)
					, imgHdc, static_cast<int>(sprite.leftTop.x), static_cast<int>(sprite.leftTop.y)
					, static_cast<int>(sprite.size.x), static_cast<int>(sprite.size.y), func);
			}
			else
			{
				TransparentBlt(hdc
					, static_cast<int>(drawX), static_cast<int>(drawY)
					, static_cast<int>(finalWidth), static_cast<int>(finalHeight)
					, imgHdc, static_cast<int>(sprite.leftTop.x), static_cast<int>(sprite.leftTop.y)
					, static_cast<int>(sprite.size.x), static_cast<int>(sprite.size.y), RGB(255, 0, 255));
			}
		}
		// PNG
		else if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Png ||
			mTexture->GetTextureType() == graphics::Texture::eTextureType::jpg)
		{
			Gdiplus::Graphics graphics(hdc);

			graphics.TranslateTransform(pos.x, pos.y);
			graphics.RotateTransform(rot);
			graphics.TranslateTransform(-pos.x, -pos.y);

			Gdiplus::Rect destRect(
				static_cast<INT>(drawX),
				static_cast<INT>(drawY),
				static_cast<INT>(finalWidth),
				static_cast<INT>(finalHeight)
			);

			graphics.DrawImage(mTexture->GetImage(),
				destRect,
				static_cast<INT>(sprite.leftTop.x),
				static_cast<INT>(sprite.leftTop.y),
				static_cast<INT>(sprite.size.x),
				static_cast<INT>(sprite.size.y),
				Gdiplus::UnitPixel, nullptr);
		}

		/*float finalX = pos.x - (sprite.size.x / 2.0f) + sprite.offset.x;
		float finalY = pos.y - (sprite.size.y / 2.0f) + sprite.offset.y;

		float finalWidth = sprite.size.x * scale.x;
		float finalHeight = sprite.size.y * scale.y;

		Rectangle(hdc,
			static_cast<int>(finalX),
			static_cast<int>(finalY),
			static_cast<int>(finalX + finalWidth),
			static_cast<int>(finalY + finalHeight)
		);*/
		float centerX = drawX + (finalWidth / 2.0f);
		float centerY = drawY + (finalHeight / 2.0f);

		int halfSize = 3;
		::Rectangle(hdc,
			static_cast<int>(centerX - halfSize),
			static_cast<int>(centerY - halfSize),
			static_cast<int>(centerX + halfSize),
			static_cast<int>(centerY + halfSize)
		);
	}
	void Animation::CreateAnimation(const std::wstring& name, graphics::Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, float duration)
	{
		mTexture = spriteSheet;

		for (size_t i = 0; i < spriteLength; i++)
		{
			Sprite sprite = {};
			sprite.leftTop.x = leftTop.x + (size.x * i);
			sprite.leftTop.y = leftTop.y;
			sprite.size = size;
			sprite.offset = offset;
			sprite.duration = duration;

			mAnimationSheet.push_back(sprite);
		}
	}
	void Animation::Reset()
	{
		mTime = 0.0f;
		mIndex = 0;
		mbComplete = false;

	}
}
