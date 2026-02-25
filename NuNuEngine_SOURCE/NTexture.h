#pragma once
#include "NResource.h"

namespace NuNu::graphics
{
	class Texture : public Resource
	{
	public:
		enum class eTextureType
		{
			Bmp,
			Png,
			jpg,
			None,
		};
		Texture();
		~Texture();

		virtual HRESULT Load(const std::wstring& path) override;

		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		HDC GetHdc() { return mHdc; }
		eTextureType GetTextureType() { return mType; }
		Gdiplus::Image* GetImage() { return mImage; }

	private:
		eTextureType mType;

		Gdiplus::Image* mImage; // PNG
		HBITMAP mBitmap; // BMP 파일
		HDC mHdc; // BMP 파일 적용할 hdc

		UINT mWidth;
		UINT mHeight;
	};
}

