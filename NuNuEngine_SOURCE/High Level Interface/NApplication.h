#pragma once
#include "GameObject/NGameObject.h"
#include "Graphics/GraphicDevice/NGraphicDevice_DX11.h"

namespace NuNu
{

	class Application
	{
	public:
		Application();
		~Application();

		void Initialize(HWND hwnd, UINT width, UINT height);
		void AdjustWindowRect(HWND hwnd, UINT width, UINT height);
		void InitializeEtc();

		void Run();

		void Update();
		void LateUpdate();
		void Render();
		void Present();
		void Destroy();
		void Release();

		[[nodiscard]] HWND GetHwnd() const { return mHwnd; }

		[[nodiscard]] UINT GetWidth() const { return mWidth; }
		[[nodiscard]] UINT GetHeight() const { return mHeight; }

		[[nodiscard]] bool IsLoaded() const { return mbLoaded; }
		[[noreturn]] void IsLoaded(const bool load) { mbLoaded = load; }

	private:
		bool mbLoaded;

		std::unique_ptr<graphics::GraphicDevice_DX11> mGraphicDevice;

		HWND mHwnd;

		UINT mWidth;
		UINT mHeight;

		// std::vector<Scene*> mScenes;
	};

}


