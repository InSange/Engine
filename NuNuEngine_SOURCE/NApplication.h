#pragma once
#include "NGameObject.h"

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
	public:
		static void AddBullet(BulletObject* bullet)
		{
			mBullets.push_back(bullet);
		}

	private:
		void clearRenderTarget();
		void copyRenderTarget(HDC source, HDC dest);
		void adjustWindowRect(HWND hwnd, UINT width, UINT height);
		void createBuffer(UINT width, UINT height);
		void initializeEtc();

	private:
		HWND mHwnd;
		HDC mHdc;
		HDC mBackHdc;

		HBITMAP mBackBitMap;

		UINT mWidth;
		UINT mHeight;

		std::vector<GameObject*> mGameObjects;

		static std::vector<BulletObject*> mBullets;
	};

}
