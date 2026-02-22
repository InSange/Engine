#include "NApplication.h"

namespace NuNu
{
	Application::Application()
		: mHwnd(nullptr)
		, mHdc(nullptr)
	{

	}

	Application::~Application()
	{

	}

	void Application::Initialize(HWND hwnd) //HWND는 포인터 주소로 연결되어 있음
	{
		mHwnd = hwnd;
		mHdc = GetDC(mHwnd);

		mPlayer.SetPosition(0, 0);
	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();
	}

	void Application::Update() // 로직 갱신
	{
		mPlayer.Update();
	}

	void Application::LateUpdate()
	{
	}

	void Application::Render() // 화면 그리기
	{
		//RECT rect = { 0, 0, 1920, 1080 };
		//HBRUSH bgBrush = CreateSolidBrush(RGB(255, 255, 255));

		//FillRect(mHdc, &rect, bgBrush);
		//DeleteObject(bgBrush);

		mPlayer.Render(mHdc);
	}
}