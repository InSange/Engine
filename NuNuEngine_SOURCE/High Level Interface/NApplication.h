#pragma once
#include "GameObject/NGameObject.h"
#include "../NGraphicDevice_DX11.h"

namespace NuNu
{

	class Application
	{
	public:
		Application();
		~Application();

		void Initialize(HWND hwnd, UINT width, UINT height);
		void Run();

		void Update();
		void LateUpdate();
		void Render();
		void Destroy();
		void Release();

		HWND GetHwnd() { return mHwnd; }
		HDC GetHdc() { return mHdc; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }

	private:
		void clearRenderTarget();
		void copyRenderTarget(HDC source, HDC dest);
		void adjustWindowRect(HWND hwnd, UINT width, UINT height);
		void createBuffer(UINT width, UINT height);
		void initializeEtc();

	private:
		std::unique_ptr<graphics::GraphicDevice_DX11> mGraphicDevice;

		HWND mHwnd;
		HDC mHdc;
		HDC mBackHdc;

		HBITMAP mBackBitMap;

		UINT mWidth;
		UINT mHeight;

		// std::vector<Scene*> mScenes;
	};

}

