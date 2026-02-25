#include "NApplication.h"
#include "NInput.h"
#include "NTime.h"
#include "NSceneManager.h"

namespace NuNu
{
	Application::Application()
		: mHwnd(nullptr)
		, mHdc(nullptr)
		, mWidth(0)
		, mHeight(0)
		, mBackHdc(nullptr)
		, mBackBitMap(nullptr)
	{

	}

	Application::~Application()
	{
		DeleteDC(mBackHdc); // 이중 hdc 삭제
		DeleteObject(mBackBitMap); // 이중 비트맵 삭제
		ReleaseDC(mHwnd, mHdc);	// 본체 반환
	}

	void Application::Initialize(HWND hwnd, UINT width, UINT height) //HWND는 포인터 주소로 연결되어 있음
	{
		adjustWindowRect(hwnd, width, height);
		createBuffer(width, height);
		initializeEtc();

		SceneManager::Initialize();
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

		SceneManager::Update();
	}

	void Application::LateUpdate()
	{
		SceneManager::LateUpdate();
	}

	void Application::Render() // 화면 그리기
	{
		clearRenderTarget();

		Time::Render(mBackHdc);
		SceneManager::Render(mBackHdc);

		copyRenderTarget(mBackHdc, mHdc);
	}

	void Application::clearRenderTarget()
	{
		//Rectangle(mBackHdc, 0, 0, 1600, 900);
		RECT bgRect = { 0, 0, 1600, 900 };
		HBRUSH bgBrush = (HBRUSH)GetStockObject(WHITE_BRUSH); // 윈도우 기본 하얀색 브러쉬 (DeleteObject 필요 없음)
		FillRect(mBackHdc, &bgRect, bgBrush);
	}

	void Application::copyRenderTarget(HDC source, HDC dest)
	{
		BitBlt(dest, 0, 0, mWidth, mHeight, source, 0, 0, SRCCOPY);
	}

	void Application::adjustWindowRect(HWND hwnd, UINT width, UINT height)
	{
		mHwnd = hwnd;
		mHdc = GetDC(mHwnd);

		RECT rect = { 0, 0, width, height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		mWidth = rect.right - rect.left;
		mHeight = rect.bottom - rect.top;

		SetWindowPos(mHwnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0);
		ShowWindow(mHwnd, true);
	}

	void Application::createBuffer(UINT width, UINT height)
	{
		// 윈도우 해상도에 맞는 백버퍼 비트맵 - 기존 HDC와 같은 비트맵
		mBackBitMap = CreateCompatibleBitmap(mHdc, width, height);
		// 백버퍼를 가르킬 DC 생성 - 기존 HDC랑 같은 환경 모습의 hdc
		mBackHdc = CreateCompatibleDC(mHdc);

		// backhdc가 지니고 있는 비트맵을 버리고 백버퍼로 갈아끼움.
		// oldbitmap에는 backhdc의 기존 비트맵이 들어가고, backhdc 기존 비트맵 자리에 backbuffer가 들어감
		HBITMAP oldBitmap = (HBITMAP)SelectObject(mBackHdc, mBackBitMap);
		DeleteObject(oldBitmap); // 기존건 필요없어졌기에 메모리 누수 방지
	}

	void Application::initializeEtc()
	{
		Input::Initailize();
		Time::Initailize();
	}
}