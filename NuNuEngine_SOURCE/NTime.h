#pragma once
#include "CommonInclude.h"

namespace NuNu
{
	class Time
	{
	public:
		static void Initailize();
		static void Tick();
		static void Render(HDC hdc);

		static float DeltaTime() { return deltaTime; }

	private:
		static LARGE_INTEGER mCpuFrequency; // CPU 고유 진동수
		static LARGE_INTEGER mPrevFrequency; // 이전 진동수
		static LARGE_INTEGER mCurrentFrequency; // 현재 진동수

		static float deltaTime;
	};
}
