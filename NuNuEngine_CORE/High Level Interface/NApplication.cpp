#include "High Level Interface/NApplication.h"
#include "Helpers/NInput.h"
#include "Helpers/NTime.h"
#include "Scene/NSceneManager.h"
#include "Resource/NResources.h"
#include "Collision/NCollisionManager.h"
#include "UI/NUIManager.h"
#include "Fmod/NFmod.h"
#include "Renderer/NRenderer.h"
#include "Event/NApplicationEvent.h"
#include "Event/NMouseEvent.h"
#include "Component/Transform/NTransform.h"

namespace NuNu
{
	Application::Application()
		: mbLoaded(false)
		, mbRunning(false)
	{
		mWindow.SetEventCallBack(N_BIND_EVENT_FN(Application::OnWindowEvent));
	}

	Application::~Application()
	{
	}

	void Application::Initialize(HWND hwnd, int width, int height) //HWND는 포인터 주소로 연결되어 있음
	{
		mWindow.SetHwnd(hwnd);
		AdjustWindowRect(hwnd, width, height);
		InitializeEtc();

		mGraphicDevice = std::make_unique<GraphicDevice_DX11>();
		mGraphicDevice->Initialize();
		renderer::Initialize();

		Fmod::Initialize();
		CollisionManager::Initialize();
		UIManager::Initialize();
		SceneManager::Initialize();

		mbRunning = true;
	}

	void Application::InitializeWindow(HWND hwnd)
	{
		SetWindowPos(hwnd, nullptr, mWindow.GetXPos(), mWindow.GetYPos()
			, mWindow.GetWindowWidth(), mWindow.GetWindowHeight(), 0);
		ShowWindow(hwnd, SW_SHOWDEFAULT);
	}

	void Application::AdjustWindowRect(HWND hwnd, int width, int height)
	{
		RECT rect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
		::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		RECT winRect;
		::GetWindowRect(mWindow.GetHwnd(), &winRect);

		//window position
		mWindow.SetPos(winRect.left, winRect.top);

		// window size
		mWindow.SetWindowWidth(rect.right - rect.left);
		mWindow.SetWindowHeight(rect.bottom - rect.top);

		//client size
		mWindow.SetWidth(width);
		mWindow.SetHeight(height);

		InitializeWindow(hwnd);
	}

	void Application::ResizeGraphicDevice(WindowResizeEvent& e)
	{
		if (mGraphicDevice == nullptr)
			return;

		// 창이 최소화되거나 아직 초기화 전이면 크기가 0 -> 무시
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
			return;

		// FrameBuffer가 아직 생성되지 않은 초기화 시점이어도 안전하게 무시
		if (renderer::FrameBuffer == nullptr)
			return;

		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(e.GetWidth());
		viewport.Height = static_cast<float>(e.GetHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		mWindow.SetWidth(viewport.Width);
		mWindow.SetHeight(viewport.Height);

		mGraphicDevice->Resize(viewport);
		renderer::FrameBuffer->Resize(viewport.Width, viewport.Height);
	}


	void Application::InitializeEtc()
	{
		Input::Initialize();
		Time::Initialize();

	}

	void Application::OnWindowEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) -> bool
			{
				ResizeGraphicDevice(e);
				return true;
			});
	}

	void Application::Run()
	{
		if (mbLoaded == false)
			mbLoaded = true;

		Update();
		LateUpdate();
		Render();

		EndOfFrame();
	}

	void Application::Close()
	{
		mbRunning = false;
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
		GetDevice<GraphicDevice_DX11>()->ClearRenderTargetView();
		GetDevice<GraphicDevice_DX11>()->ClearDepthStencilView();
		GetDevice<GraphicDevice_DX11>()->BindViewPort();
		GetDevice<GraphicDevice_DX11>()->BindDefaultRenderTarget();

		Time::Render();
		SceneManager::Render();
		CollisionManager::Render();
		UIManager::Render();

		//copy back buffer
		Microsoft::WRL::ComPtr<ID3D11Texture2D> src = GetDevice<GraphicDevice_DX11>()->GetFrameBuffer();
		Microsoft::WRL::ComPtr<ID3D11Texture2D> dst = renderer::FrameBuffer->GetAttachmentTexture(0)->GetTexture();

		GetDevice<GraphicDevice_DX11>()->CopyResource(dst.Get(), src.Get());
	}

	void Application::Present()
	{
		GetDevice<GraphicDevice_DX11>()->Present();
	}

	void Application::EndOfFrame()
	{
		SceneManager::EndOfFrame();
	}

	void Application::Release()
	{
		SceneManager::Release();
		UIManager::Release();
		Resources::Release();

		renderer::Release();
	}
}

