#include "NGraphicDevice_DX11.h"
#include "./High Level Interface/NApplication.h"

extern NuNu::Application application;

namespace NuNu
{
	graphics::GraphicDevice_DX11::GraphicDevice_DX11()
	{
	}

	graphics::GraphicDevice_DX11::~GraphicDevice_DX11()
	{
	}

	void graphics::GraphicDevice_DX11::Initialize()
	{
		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE,
			0, creationFlags,
			featureLevels, ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, mDevice.GetAddressOf(),
			0, mContext.GetAddressOf());

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		swapChainDesc.OutputWindow = application.GetHwnd();	// 어느 윈도우에 해당 화면을 띄울것인지 세팅
		swapChainDesc.Windowed = true;	// 전체화면이 아닌 창모드 실행
		swapChainDesc.BufferCount = 2;	// 삼중 버퍼(3)도 가능하지만 이중 버퍼링 사용
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// 창모드 <-> 전체 화면 전환을 허용
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 이전 프레임 장면을 유지하지 않는다.

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 렌더 타겟으로 사용
		swapChainDesc.BufferDesc.Width = application.GetWidth();	// 백버퍼의 너비
		swapChainDesc.BufferDesc.Height = application.GetHeight();	// 백버퍼의 높이
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 백버퍼의 포맷
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 144;	// 수직 동기화
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		// 4X MSAA surported check
		UINT quility = 0;
		mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quility);
		//if (quility > 0)
		//{
		//	swapChainDesc.SampleDesc.Count = 4; // how many multisamples
		//	swapChainDesc.SampleDesc.Quality = quility - 1;
		//}

		swapChainDesc.SampleDesc.Count = 1; // how many multisamples
		swapChainDesc.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice>     pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter>    pAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory>    pFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return;

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
			return;

		if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
			return;

		if (FAILED(pFactory->CreateSwapChain(mDevice.Get(), &swapChainDesc, mSwapChain.GetAddressOf())))
			return;

		mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)mRenderTarget.GetAddressOf());
		mDevice->CreateRenderTargetView(mRenderTarget.Get(), nullptr, mRenderTargetView.GetAddressOf());

		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		depthStencilDesc.Width = application.GetWidth();
		depthStencilDesc.Height = application.GetHeight();
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;


		if (FAILED(mDevice->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencil.GetAddressOf())))
			return;

		if (FAILED(mDevice->CreateDepthStencilView(mDepthStencil.Get(), nullptr, mDepthStencilView.GetAddressOf())))
			return;

		int a = 0;
	}

	void graphics::GraphicDevice_DX11::Draw()
	{
		FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);


		mSwapChain->Present(1, 0);
	}
}
