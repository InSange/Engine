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

		static Texture* Create(const std::wstring& name, UINT width, UINT height);

		Texture();
		~Texture();

		virtual HRESULT Load(const std::wstring& path) override;

		UINT GetWidth() { return mWidth; }
		void SetWidth(UINT width) { mWidth = width; }
		UINT GetHeight() { return mHeight; }
		void SetHeight(UINT height) { mHeight = height; }
		HDC GetHdc() { return mHdc; }
		eTextureType GetTextureType() { return mType; }
		Gdiplus::Image* GetImage() { return mImage; }

		bool IsAlpha() { return mbAlpha; }

	private:
		bool mbAlpha;
		eTextureType mType;

		Gdiplus::Image* mImage; // PNG
		HBITMAP mBitmap; // BMP 파일
		HDC mHdc; // BMP 파일 적용할 hdc

		UINT mWidth;
		UINT mHeight;
	};
}

