#pragma once
#include "CommonInclude.h"

namespace NuNu
{

	class Application
	{
	public:
		Application();
		~Application();

		void Initialize(HWND hwnd);
		void Run();

		void Update();
		void LateUpdate();

		void Render();//그리기
	private:
		HWND mHwnd;
		HDC mHdc;

		float mSpeed;
		float mX;
		float mY;
	};

}
