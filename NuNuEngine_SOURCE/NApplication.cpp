#include "NApplication.h"
#include "NInput.h"
#include "NTime.h"

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

		Input::Initailize();
		Time::Initailize();
	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();
	}

	void Application::Update() // 로직 갱신
	{
		Input::Update();
		Time::Tick();

		mPlayer.Update();
	}

	void Application::LateUpdate()
	{
	}

	void Application::Render() // 화면 그리기
	{
		Time::Render(mHdc);
		mPlayer.Render(mHdc);
	}
}