#include "High Level Interface/NApplication.h"
#include "Helpers/NInput.h"
#include "Helpers/NTime.h"
#include "Scene/NSceneManager.h"
#include "Resource/NResources.h"
#include "Collision/NCollisionManager.h"
#include "UI/NUIManager.h"
#include "Fmod/NFmod.h"
#include "Renderer/NRenderer.h"

namespace NuNu
{
	Application::Application()
		: mHwnd(nullptr)
		, mHdc(nullptr)
		, mWidth(0)
		, mHeight(0)
		, mBackHdc(nullptr)
		, mBackBitMap(nullptr)
		, mbLoaded(false)
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
		AdjustWindowRect(hwnd, width, height);
		InitializeEtc();

		mGraphicDevice = std::make_unique<graphics::GraphicDevice_DX11>();
		//renderer::Initialize();
		mGraphicDevice->Initialize();

		Fmod::Initialize();
		CollisionManager::Initialize();
		UIManager::Initialize();
		SceneManager::Initialize();
	}

	void Application::AdjustWindowRect(HWND hwnd, UINT width, UINT height)
	{
		mHwnd = hwnd;
		mHdc = GetDC(hwnd);

		RECT rect = { 0, 0, (LONG)width, (LONG)height };
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		mWidth = rect.right - rect.left;
		mHeight = rect.bottom - rect.top;

		SetWindowPos(hwnd, nullptr, 0, 0, mWidth, mHeight, 0);
		ShowWindow(hwnd, true);
	}

	void Application::InitializeEtc()
	{
		Input::Initailize();
		Time::Initailize();
	}

	void Application::Run()
	{
		if (mbLoaded == false)
			mbLoaded = true;

		Update();
		LateUpdate();
		Render();

		Destroy();
	}

	void Application::Update() // 로직 갱신
	{
		Input::Update();
		Time::Tick();

		CollisionManager::Update();
		UIManager::Update();
		SceneManager::Update();
	}

	void Application::LateUpdate()
	{
		CollisionManager::LateUpdate();
		UIManager::LateUpdate();
		SceneManager::LateUpdate();
	}

	void Application::Render() // 화면 그리기
	{
		graphics::GetDevice()->ClearRenderTargetView();
		graphics::GetDevice()->ClearDepthStencilView();
		graphics::GetDevice()->BindViewPort();
		graphics::GetDevice()->BindDefaultRenderTarget();

		Time::Render();
		CollisionManager::Render();
		UIManager::Render();
		SceneManager::Render();

		graphics::GetDevice()->Present();
	}

	void Application::Destroy()
	{
		SceneManager::Destroy();
	}

	void Application::Release()
	{
		SceneManager::Release();
		UIManager::Release();
		Resources::Release();

		renderer::Release();
	}
}

