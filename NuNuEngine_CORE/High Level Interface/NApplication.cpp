#include "High Level Interface/NApplication.h"
#include "Helpers/NInput.h"
#include "Helpers/NTime.h"
#include "Scene/NSceneManager.h"
#include "Resource/NResources.h"
#include "Collision/NCollisionManager.h"
#include "Stage/NStageManager.h"
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

		mGraphicDevice_12 = std::make_unique<graphics::GraphicDevice_DX12>();
		mGraphicDevice_12->Initialize();
		renderer::Initialize();

		Fmod::Initialize();
		CollisionManager::Initialize();
		UIManager::Initialize();
		SceneManager::Initialize();

		mbRunning = true;
	}

	void Application::WaitforGpu()
	{
		mGraphicDevice_12->WaitForGpu();
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
		if (GetDevice() == nullptr) return;
		GetDevice()->ScheduleResize(e.GetWidth(), e.GetHeight());
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
		StageManager::Update();
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
		GetDevice()->ResetCommandAllocator();
		GetDevice()->ResetCommandList();
		GetDevice()->SetBaseGraphicsRootSignature();
		GetDevice()->BindViewportAndScissor();
		GetDevice()->TranstionResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		GetDevice()->BindFrameBuffer();

		Time::Render();
		SceneManager::Render();
		CollisionManager::Render();
		UIManager::Render();

		GetDevice()->Render();
	}

	void Application::ExcuteCommandList()
	{
		GetDevice()->ExcuteCommandList();
	}

	void Application::CloseCommandList()
	{
		GetDevice()->CloseCommandList();
	}

	void Application::Present()
	{
		GetDevice()->Present();
	}

	void Application::SignalFrameCompletion()
	{
		GetDevice()->SignalFrameCompletion();
	}

	void Application::WaitForNextFrameResources()
	{
		GetDevice()->WaitForNextFrameResources();
	}

	void Application::MoveToNextFrame()
	{
		GetDevice()->MoveToNextFrame();
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

		Fmod::Release();
		renderer::Release();
	}
}
