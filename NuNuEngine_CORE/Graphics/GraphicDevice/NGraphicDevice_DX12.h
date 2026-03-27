#pragma once
#include "Graphics/NGraphics.h"
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace NuNu::graphics
{
	struct FrameContext
	{
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator;
		UINT64 FenceValue = 0;
	};

	class GraphicDevice_DX12
	{
	public:
		GraphicDevice_DX12();
		~GraphicDevice_DX12();

		bool CreateDevice();
		void GetHardwareAdapter(_In_ IDXGIFactory1* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter = false);
		void Initialize();
		void WaitForPreviousFrame();
		FrameContext* WaitForNextFrameResources();
		void PopulateCommandList();
		void ExcuteCommandList();
		void Render();
		void CloseCommandList();
		void Present();

		Microsoft::WRL::ComPtr<ID3D12Device> GetID3D12Device() { return mDevice; }
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue() { return mCommandQueue; }
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSrvHeap() { return mSrvHeap; }
		Microsoft::WRL::ComPtr<IDXGISwapChain3> GetSwapChain() { return mSwapChain; }
		Microsoft::WRL::ComPtr<ID3D12Resource> GetRenderTargetResource(int idx) { return mRenderTargets[idx]; }
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRTVHeap() { return mRtvHeap; }

		//imgui
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return mCommandList; }
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRnderTargetDescriptorHandle(int idx) { return mRenderTragetDesciptorHandle[idx]; }

	private:
		bool mbUseWarpDevice;

		Microsoft::WRL::ComPtr<ID3D12Device>			mDevice;
		Microsoft::WRL::ComPtr<IDXGIFactory4>			mFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain3>			mSwapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	mRtvHeap;
		UINT											mRtvDescriptorSize;
		Microsoft::WRL::ComPtr<ID3D12Resource>			mRenderTargets[2];
		CD3DX12_CPU_DESCRIPTOR_HANDLE mRenderTragetDesciptorHandle[2];

		Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> mPipelineState;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue>		mCommandQueue;
		//Microsoft::WRL::ComPtr<ID3D12CommandAllocator>	mCommandAllocator;

		FrameContext mFrameContext[2]; //double buffering

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

		UINT											mFrameIndex;

		// App resources.
		Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;

		Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
		UINT64 mFenceValue;
		HANDLE mFenceEvent;

		//imgui 
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap;
	};

	inline GraphicDevice_DX12*& GetDevice()
	{
		static GraphicDevice_DX12* device = nullptr;
		return device;
	}
}
