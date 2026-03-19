#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

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
}

