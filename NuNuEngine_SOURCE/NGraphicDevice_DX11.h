#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "../NuNuEngine_SOURCE/Common/CommonInclude.h"

namespace NuNu::graphics
{
	class GraphicDevice_DX11
	{
	public:
		GraphicDevice_DX11();
		~GraphicDevice_DX11();

		bool CreateDevice();
		bool CreateSwapchain(DXGI_SWAP_CHAIN_DESC desc);
		bool GetBuffer(UINT Buffer, REFIID riid, void** ppSurface);
		bool CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView);
		bool CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView);
		bool CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D);
		bool CreateVertexShader(const std::wstring& fileName, ID3DBlob** ppCode, ID3D11VertexShader** ppVertexShader);
		bool CreatePixelShader(const std::wstring& fileName, ID3DBlob** ppCode, ID3D11PixelShader** ppPixelShader);
		bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements
			, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout);
		bool CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer);

		void BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);

		void Initialize();
		void Draw();

	private:	// COM 스마트 포인터는 직접 Release를 호출하지 않아도 자동으로 GPU 메모리를 깔끔하게 해재해 주는 스마트 포인터
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;	// GPU를 제어하는 핵심 객체
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;	// GPU에게 명령을 내리는 객체
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mRenderTarget;	// 렌더 타겟
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;	// 렌더 타겟 뷰 -> 백버퍼에 렌더링된 결과를 화면에 띄우기 위한 뷰
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			mDepthStencil;	// 깊이 스텐실 -> 2D는 우선순위, 3D는 게임에서 물체의 앞뒤를 구분하기 위한 스텐실 (Z버퍼)
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;	// 깊이 스텐실 뷰

		Microsoft::WRL::ComPtr<IDXGISwapChain>	mSwapChain;	// 화면에 그리기 위한 스왑 체인
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplers;	// 샘플러 Linear(부드럽게), Point(각지게), Anisotropic(최고 품질)
	};

	inline GraphicDevice_DX11*& GetDevice()
	{
		static GraphicDevice_DX11* device = nullptr;

		return device;
	}
}

