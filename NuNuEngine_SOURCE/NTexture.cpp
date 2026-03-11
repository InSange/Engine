#include "NTexture.h"
#include "NApplication.h"
#include "NResources.h"

extern NuNu::Application application;

namespace NuNu::graphics
{
	Texture* Texture::Create(const std::wstring& name, UINT width, UINT height)
	{
		std::wstring keyName = name;
		std::wstring ext = L"";

		size_t dotPos = name.find_last_of(L".");
		if (dotPos != std::wstring::npos) 
		{
			keyName = name.substr(0, dotPos);
			ext = name.substr(dotPos + 1);
		}

		Texture* image = Resources::Find<Texture>(keyName);

		if (image != nullptr)
			return image;

		image = new Texture();
		image->SetName(keyName);
		image->SetWidth(width);
		image->SetHeight(height);

		if (ext == L"bmp")
		{
			image->mType = eTextureType::Bmp;

			HDC hdc = application.GetHdc();

			image->mBitmap = CreateCompatibleBitmap(hdc, width, height);
			image->mHdc = CreateCompatibleDC(hdc);

			HBITMAP oldBitmap = (HBITMAP)SelectObject(image->mHdc, image->mBitmap);
			DeleteObject(oldBitmap);
		}
		else if (ext == L"png" || ext == L"jpg")
		{
			image->mType = eTextureType::Png;

			image->mImage = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
		}

		Resources::Insert(keyName, image);

		return image;
	}

	Texture::Texture()
		: Resource(enums::eResourceType::Texture)
		, mbAlpha(false)
		, mType(eTextureType::None)
		, mImage(nullptr)
		, mBitmap(NULL)
		, mHdc(NULL)
		, mWidth(0)
		, mHeight(0)
	{
	}

	Texture::~Texture()
	{
		if (mImage != nullptr)
		{
			delete mImage;
			mImage = nullptr;
		}

		if (mBitmap != NULL)
		{
			DeleteObject(mBitmap);
			mBitmap = NULL;
		}

		if (mHdc != NULL)
		{
			DeleteDC(mHdc);
			mHdc = NULL;
		}
	}
	HRESULT Texture::Load(const std::wstring& path)
	{
		std::wstring ext = path.substr(path.find_last_of(L".") + 1);
		// bmp
		if (ext == L"bmp")
		{
			mType = eTextureType::Bmp;
			mBitmap = (HBITMAP)LoadImageW(nullptr, path.c_str(), IMAGE_BITMAP,
				0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

			if (mBitmap == nullptr) return S_FALSE;

			BITMAP info = {};
			GetObject(mBitmap, sizeof(BITMAP), &info);

			mWidth = info.bmWidth;
			mHeight = info.bmHeight;

			if (info.bmBitsPixel == 32)
				mbAlpha = true;
			else if (info.bmBitsPixel == 24)
				mbAlpha = false;

			HDC mainDC = application.GetHdc();
			mHdc = CreateCompatibleDC(mainDC);

			HBITMAP oldBitmap = (HBITMAP)SelectObject(mHdc, mBitmap);
			DeleteObject(oldBitmap);
		}
		else if (ext == L"png")
		{
			mType = eTextureType::Png;
			mImage = Gdiplus::Image::FromFile(path.c_str());

			if (mImage == nullptr) return S_FALSE;

			mWidth = mImage->GetWidth();
			mHeight = mImage->GetHeight();
		}
		else if (ext == L"jpg")
		{
			mType = eTextureType::jpg;
			mImage = Gdiplus::Image::FromFile(path.c_str());

			if (mImage == nullptr) return S_FALSE;

			mWidth = mImage->GetWidth();
			mHeight = mImage->GetHeight();
		}

		return S_OK;
	}
}