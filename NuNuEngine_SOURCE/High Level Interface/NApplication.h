#pragma once
#include "GameObject/NGameObject.h"

#ifdef DX12_ENABLED
#include "Graphics/GraphicDevice/NGraphicDevice_DX12.h"
#else
#include "Graphics/GraphicDevice/NGraphicDevice_DX11.h"
#endif

#include "Event/NApplicationEvent.h"
#include "Event/NMouseEvent.h"
#include "Event/NKeyEvent.h"
#include "Event/NEvent.h"
#include "NWindow.h"

namespace NuNu
{

	class Application
	{
	public:
		Application();
		~Application();

		void Initialize(HWND hwnd, int width, int height);
		void InitializeWindow(HWND hwnd);
		void AdjustWindowRect(HWND hwnd, int width, int height);
		void ResizeGraphicDevice(WindowResizeEvent& e);
		void InitializeEtc();

		void OnWindowEvent(Event& e);

		void Run();
		void Close();

		void Update();
		void LateUpdate();
		void Render();
		void Present();
		void EndOfFrame();
		void Release();

		Window& GetWindow() { return mWindow; }
		bool IsLoaded() const { return mbLoaded; }
		void IsLoaded(const bool load) { mbLoaded = load; }
		bool IsRunning() const { return mbRunning; }

	private:
		bool mbLoaded;
		bool mbRunning;

		std::unique_ptr<graphics::GraphicDevice_DX11> mGraphicDevice;

		Window mWindow;
	};

}


