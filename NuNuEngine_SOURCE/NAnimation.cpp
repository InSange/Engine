#include "NAnimation.h"
#include "NTime.h"
#include "NTransform.h"
#include "NGameObject.h"
#include "NAnimator.h"
#include "NRenderer.h"

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
					, pos.x - (sprite.size.x / 2.0f) + sprite.offset.x, pos.y - (sprite.size.y / 2.0f) + sprite.offset.y
					, sprite.size.x * scale.x, sprite.size.y * scale.y
					, imgHdc, sprite.leftTop.x, sprite.leftTop.y, sprite.size.x, sprite.size.y, func);
			}
			else
			{
				TransparentBlt(hdc 
					, pos.x - (sprite.size.x / 2.0f) + sprite.offset.x, pos.y - (sprite.size.y / 2.0f) + sprite.offset.y
					, sprite.size.x, sprite.size.y, imgHdc
					, sprite.leftTop.x, sprite.leftTop.y, sprite.size.x, sprite.size.y, RGB(255, 0, 255));
			}
		}
		else if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Png ||
			mTexture->GetTextureType() == graphics::Texture::eTextureType::jpg)
		{

			Gdiplus::ImageAttributes imgAtt = {}; // 투명화 추가
			imgAtt.SetColorKey(Gdiplus::Color(230, 230, 230), Gdiplus::Color(255, 255, 255));

			Gdiplus::Graphics graphics(hdc);

			graphics.TranslateTransform(pos.x, pos.y);
			graphics.RotateTransform(rot);
			graphics.TranslateTransform(-pos.x, -pos.y);

			Gdiplus::Rect destRect(
				static_cast<INT>(pos.x - (sprite.size.x / 2.0f)),
				static_cast<INT>(pos.y - (sprite.size.y / 2.0f)),
				static_cast<INT>(sprite.size.x * scale.x),
				static_cast<INT>(sprite.size.y * scale.y)
			);

			graphics.DrawImage(mTexture->GetImage(),
				destRect,
				static_cast<INT>(sprite.leftTop.x),
				static_cast<INT>(sprite.leftTop.y),
				static_cast<INT>(sprite.size.x),
				static_cast<INT>(sprite.size.y),
				Gdiplus::UnitPixel, nullptr/*&imgAtt*/);
		}
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