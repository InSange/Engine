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

		void Render();//그리기

	public:
		static void AddBullet(BulletObject* bullet)
		{
			mBullets.push_back(bullet);
		}

	private:
		HWND mHwnd;
		HDC mHdc;
		HDC mBackHdc;

		HBITMAP mBackBitMap;

		UINT mWidth;
		UINT mHeight;

		GameObject mPlayer;

		static std::vector<BulletObject*> mBullets;
	};

}
