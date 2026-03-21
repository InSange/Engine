#include "./High Level Interface/NApplication.h"
#include "Renderer/NRenderer.h"
#include "NGraphicDevice_DX11.h"
#include "Resource/Graphics/Shader/NShader.h"
#include "Resource/NResources.h"

extern NuNu::Application application;

namespace NuNu
{
	graphics::GraphicDevice_DX11::GraphicDevice_DX11()
	{
		graphics::GetDevice() = this;

		if (!(CreateDevice()))
			assert(NULL && "Create Device Failed!");
	}

	graphics::GraphicDevice_DX11::~GraphicDevice_DX11()
	{
	}

	bool graphics::GraphicDevice_DX11::CreateDevice()
	{
		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		bool hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE,
			0, creationFlags,
			featureLevels, ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, mDevice.GetAddressOf(),
			0, mContext.GetAddressOf());

		return true;
	}

	bool graphics::GraphicDevice_DX11::CreateSwapchain(DXGI_SWAP_CHAIN_DESC desc)
	{
		Microsoft::WRL::ComPtr<IDXGIDevice>     pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter>    pAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory>    pFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return false;

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
			return false;

		if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
			return false;

		if (FAILED(pFactory->CreateSwapChain(mDevice.Get(), &desc, mSwapChain.GetAddressOf())))
			return false;

		return true;
	}

	bool graphics::GraphicDevice_DX11::GetBuffer(UINT Buffer, REFIID riid, void** ppSurface)
	{
		if (FAILED(mSwapChain->GetBuffer(Buffer, riid, ppSurface)))
			return false;

		return true;
	}

	bool graphics::GraphicDevice_DX11::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
	{
		if (FAILED(mDevice->CreateRenderTargetView(pResource, pDesc, ppRTView)))
			return false;

		return true;
	}

	bool graphics::GraphicDevice_DX11::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
	{
		if (FAILED(mDevice->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView)))
			return false;

		return true;
	}

	bool graphics::GraphicDevice_DX11::CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D)
	{
		if (FAILED(mDevice->CreateTexture2D(pDesc, pInitialData, ppTexture2D)))
			return false;

		return true;
	}

	bool graphics::GraphicDevice_DX11::CreateVertexShader(const std::wstring& fileName, ID3DBlob** ppCode, ID3D11VertexShader** ppVertexShader)
	{
		DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

		ID3DBlob* errorBlob = nullptr;
		const std::wstring shaderFilePath = L"..\\Shader_Source\\";
		D3DCompileFromFile((shaderFilePath + fileName + L"VS.hlsl").c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "vs_5_0", shaderFlags, 0, ppCode, &errorBlob);

		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
			assert(NULL && "hlsl file have problem check message!");
			return false;
		}

		if (FAILED(mDevice->CreateVertexShader((*ppCode)->GetBufferPointer(), (*ppCode)->GetBufferSize(), nullptr, ppVertexShader)))
			return false;

		return true;
	}

	bool graphics::GraphicDevice_DX11::CreatePixelShader(const std::wstring& fileName, ID3DBlob** ppCode, ID3D11PixelShader** ppPixelShader)
	{
		DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

		ID3DBlob* errorBlob = nullptr;
		const std::wstring shaderFilePath = L"..\\Shader_Source\\";
		D3DCompileFromFile((shaderFilePath + fileName + L"PS.hlsl").c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", shaderFlags, 0, ppCode, &errorBlob);

		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
			assert(NULL && "hlsl file have problem check message!");
			return false;
		}

		if (FAILED(mDevice->CreatePixelShader((*ppCode)->GetBufferPointer(), (*ppCode)->GetBufferSize(), nullptr, ppPixelShader)))
			return false;

		return true;
	}

	bool graphics::GraphicDevice_DX11::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
	{
		if (FAILED(mDevice->CreateInputLayout(pInputElementDescs, NumElements
			, pShaderBytecodeWithInputSignature
			, BytecodeLength
			, ppInputLayout)))
			return false;

		return true;
	}

	bool graphics::GraphicDevice_DX11::CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer)
	{
		if (FAILED(mDevice->CreateBuffer(pDesc, pInitialData, ppBuffer)))
			return false;

		return true;
	}

	void graphics::GraphicDevice_DX11::BindVS(ID3D11VertexShader* pVertexShader)
	{
		mContext->VSSetShader(pVertexShader, 0, 0);
	}

	void graphics::GraphicDevice_DX11::BindPS(ID3D11PixelShader* pPixelShader)
	{
		mContext->PSSetShader(pPixelShader, 0, 0);
	}

	void graphics::GraphicDevice_DX11::BindVertexBuffer(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)
	{
		mContext->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
	}

	void graphics::GraphicDevice_DX11::BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
	{
		UINT slot = (UINT)type;
		switch (stage)
		{
		case NuNu::graphics::eShaderStage::VS:
			mContext->VSSetConstantBuffers(slot, 1, &buffer);
			break;
		case NuNu::graphics::eShaderStage::HS:
			mContext->HSSetConstantBuffers(slot, 1, &buffer);
			break;
		case NuNu::graphics::eShaderStage::DS:
			mContext->DSSetConstantBuffers(slot, 1, &buffer);
			break;
		case NuNu::graphics::eShaderStage::GS:
			mContext->GSSetConstantBuffers(slot, 1, &buffer);
			break;
		case NuNu::graphics::eShaderStage::PS:
			mContext->PSSetConstantBuffers(slot, 1, &buffer);
			break;
		case NuNu::graphics::eShaderStage::CS:
			mContext->CSSetConstantBuffers(slot, 1, &buffer);
			break;
		case NuNu::graphics::eShaderStage::All:
			mContext->VSSetConstantBuffers(slot, 1, &buffer);
			mContext->HSSetConstantBuffers(slot, 1, &buffer);
			mContext->DSSetConstantBuffers(slot, 1, &buffer);
			mContext->GSSetConstantBuffers(slot, 1, &buffer);
			mContext->PSSetConstantBuffers(slot, 1, &buffer);
			mContext->CSSetConstantBuffers(slot, 1, &buffer);
			break;
		default:
			break;
		}
	}

	void graphics::GraphicDevice_DX11::Initialize()
	{
#pragma region swapchain desc
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

#pragma region 4X MSAA surported check
		// 4X MSAA surported check
		UINT quility = 0;
		mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quility);
		//if (quility > 0)
		//{
		//	swapChainDesc.SampleDesc.Count = 4; // how many multisamples
		//	swapChainDesc.SampleDesc.Quality = quility - 1;
		//}

#pragma endregion

		swapChainDesc.SampleDesc.Count = 1; // how many multisamples
		swapChainDesc.SampleDesc.Quality = 0;

#pragma endregion
		if (!(CreateSwapchain(swapChainDesc)))
			assert(NULL && "Create Swapchain Failed!");

		if (!(GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)mRenderTarget.GetAddressOf())))
			assert(NULL && "Couldn't bring rendertarget!");

		if (!(CreateRenderTargetView(mRenderTarget.Get(), nullptr, mRenderTargetView.GetAddressOf())))
			assert(NULL && "Create RenderTargetView Failed!");

#pragma region depthstencil desc
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		depthStencilDesc.Width = application.GetWidth();
		depthStencilDesc.Height = application.GetHeight();
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

#pragma endregion
		if (!(CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencil.GetAddressOf())))
			assert(NULL && "Create depthstencil texture failed!");

		if (!(CreateDepthStencilView(mDepthStencil.Get(), nullptr, mDepthStencilView.GetAddressOf())))
			assert(NULL && "Create depthstencilview failed!");

#pragma region inputLayout Desc
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesces[2] = {};

		inputLayoutDesces[0].AlignedByteOffset = 0;
		inputLayoutDesces[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[0].InputSlot = 0;
		inputLayoutDesces[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[0].SemanticName = "POSITION";
		inputLayoutDesces[0].SemanticIndex = 0;

		inputLayoutDesces[1].AlignedByteOffset = 12;
		inputLayoutDesces[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputLayoutDesces[1].InputSlot = 0;
		inputLayoutDesces[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[1].SemanticName = "COLOR";
		inputLayoutDesces[1].SemanticIndex = 0;

#pragma endregion

		graphics::Shader* triangle = Resources::Find<graphics::Shader>(L"TriangleShader");

		if (!(CreateInputLayout(inputLayoutDesces, 2
			, triangle->GetVSBlob()->GetBufferPointer()
			, triangle->GetVSBlob()->GetBufferSize()
			, &renderer::inputLayouts)))
		{
			assert(NULL && "Create input layout failed!");
		}

		renderer::vertexBuffer.Create(renderer::vertexes);

#pragma region index buffer desc
		D3D11_BUFFER_DESC indexBufferdesc = {};
		indexBufferdesc.ByteWidth = sizeof(UINT) * renderer::indices.size();
		indexBufferdesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		indexBufferdesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferdesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA indicesData = {};
		indicesData.pSysMem = renderer::indices.data();
#pragma endregion
		if (!(graphics::GetDevice()->CreateBuffer(&indexBufferdesc, &indicesData, &renderer::indexBuffer)))
			assert(NULL && "indices buffer create fail!!");

#pragma region constant buffer desc
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.ByteWidth = sizeof(Vector4); // constant buffer 
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		Vector4 pos(0.5f, 0.0f, 0.0f, 1.0f);
		D3D11_SUBRESOURCE_DATA constantBufferData = {};
		constantBufferData.pSysMem = &pos;
#pragma endregion
		if (!(graphics::GetDevice()->CreateBuffer(&constantBufferDesc, &constantBufferData, &renderer::constantBuffer)))
			assert(NULL && "indices buffer create fail!!");
	}

	void graphics::GraphicDevice_DX11::Draw()
	{
		FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);

		mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		D3D11_VIEWPORT viewPort =
		{
			0, 0, application.GetWidth(), application.GetHeight(),
			0.0f, 1.0f
		};
		mContext->RSSetViewports(1, &viewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

		BindConstantBuffer(eShaderStage::VS, eCBType::Transform, renderer::constantBuffer);

		mContext->IASetInputLayout(renderer::inputLayouts);
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		renderer::vertexBuffer.Bind();

		// 만들어둔 인덱스 버퍼를 렌더링 파이프라인(Input Assembler)에 장착합니다.
		mContext->IASetIndexBuffer(renderer::indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		graphics::Shader* triangle = Resources::Find<graphics::Shader>(L"TriangleShader");
		triangle->Bind();

		// 단순 점 그리기(Draw)가 아니라, 인덱스 배열의 순서표에 따라 그리도록 DrawIndexed 호출
		//mContext->DrawIndexed(renderer::indices.size(), 0, 0);
		mContext->Draw(3, 0);

		mSwapChain->Present(1, 0);
	}
}

