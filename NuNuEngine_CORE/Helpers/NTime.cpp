#include "Helpers/NTime.h"

namespace NuNu
{
	LARGE_INTEGER Time::mCpuFrequency = {}; 
	LARGE_INTEGER Time::mPrevFrequency = {}; 
	LARGE_INTEGER Time::mCurrentFrequency = {}; 

	float Time::deltaTime = 0.0f;

	void Time::Initialize()
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

	void Time::Render()
	{
	}
}
