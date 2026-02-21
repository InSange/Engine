#include "NApplication.h"

namespace NuNu
{
	Application::Application()
		: mHwnd(nullptr)
		, mHdc(nullptr)
		, mSpeed(0.0f)
		, mX(0.0f)
		, mY(0.0f)
	{

	}

	Application::~Application()
	{

	}

	void Application::Initialize(HWND hwnd) //HWND는 포인터 주소로 연결되어 있음
	{
		mHwnd = hwnd;
		mHdc = GetDC(mHwnd);
	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();
	}

	void Application::Update() // 로직 갱신
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			mX -= 0.01f;
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			mX += 0.01f;
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			mY -= 0.01f;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			mY += 0.01f;
		}
	}

	void Application::LateUpdate()
	{
	}

	void Application::Render() // 화면 그리기
	{
		HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(mHdc, brush); // 반환값 HGDIOBJ 이전 브러쉬 밀어냄

		HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(mHdc, redPen);
		// 기본으로 자주 사용 되는 GDI오브젝트를 미리 DC안에 만들어둔 오브젝트 = 스톡 오브젝트.
		HBRUSH grayBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
		oldBrush = (HBRUSH)SelectObject(mHdc, grayBrush);

		Rectangle(mHdc, 100 + mX, 100 + mY, 200 + mX, 200 + mY);

		SelectObject(mHdc, oldBrush);
		SelectObject(mHdc, oldPen);
		DeleteObject(brush); // 메모리 누수 방지
		DeleteObject(redPen); // 메모리 누수 방지


	}
}