#include "NTime.h"

namespace NuNu
{
	LARGE_INTEGER Time::mCpuFrequency = {}; 
	LARGE_INTEGER Time::mPrevFrequency = {}; 
	LARGE_INTEGER Time::mCurrentFrequency = {}; 

	float Time::deltaTime = 0.0f;

	void Time::Initailize()
	{
		QueryPerformanceFrequency(&mCpuFrequency); // CPU 고유 진동수
		QueryPerformanceCounter(&mPrevFrequency); // 프로그램 시작 했을 때 현재 진동수
	}

	void Time::Tick()
	{
		QueryPerformanceCounter(&mCurrentFrequency);

		float differenceFrequency = static_cast<float>(mCurrentFrequency.QuadPart - mPrevFrequency.QuadPart);

		deltaTime = differenceFrequency / static_cast<float>(mCpuFrequency.QuadPart);

		mPrevFrequency.QuadPart = mCurrentFrequency.QuadPart;
	}

	void Time::Render(HDC hdc)
	{
		static float time = 0.0f;

		time += deltaTime;
		float fps = 1.0f / deltaTime;
		
		wchar_t str[50] = L"";
		swprintf_s(str, 50, L"Time : %f, Frame : %d", time, (int)fps); 
		int len = wcsnlen_s(str, 50);


		TextOut(hdc, 0, 0, str, len);
	}
}