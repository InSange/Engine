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
		bool CreateCommittedResource(D3D12_HEAP_PROPERTIES* pHeapProperties,
			D3D12_HEAP_FLAGS HeapFlags,
			D3D12_RESOURCE_DESC* pDesc,
			D3D12_RESOURCE_STATES InitialResourceState,
			D3D12_CLEAR_VALUE* pOptimizedClearValue,
			REFIID riidResource,
			void** ppvResource);
		bool CreateVertexShader(const std::wstring& fileName, ID3DBlob** ppCode);
		bool CreatePixelShader(const std::wstring& fileName, ID3DBlob** ppCode);
		bool CreateGraphicsPipelineState(_In_  const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc/*, void** ppPipelineState*/);
		bool CreatePipelineState(_In_ const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc, ID3D12PipelineState** ppPipelineState);

		// binding command list...
		void BindVertexBuffer(UINT StartSlot, UINT NumViews, D3D12_VERTEX_BUFFER_VIEW* pViews);
		void BindViewportAndScissor();
		void BindFrameBuffer();
		void SetBaseGraphicsRootSignature();

		// render ...
		void WaitForGpu();
		void SignalFrameCompletion();
		FrameContext* WaitForNextFrameResources();
		void MoveToNextFrame();
		void ResetCommandList();
		void ResetCommandAllocator();
		void TranstionResourceBarrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
		void PopulateCommandList();
		void DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
		void DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
		void Render();
		void CloseCommandList();
		void ExcuteCommandList();
		void Present();

		// scene view render target
		void CreateSceneRenderTarget(UINT width, UINT height);
		void BeginSceneRenderTarget();
		void EndSceneRenderTarget();
		D3D12_GPU_DESCRIPTOR_HANDLE GetSceneSRVGpuHandle() const { return mSceneSRVGpu; }

		Microsoft::WRL::ComPtr<ID3D12Device> GetID3D12Device() { return mDevice; }
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue() { return mCommandQueue; }
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSrvHeap() { return mSrvHeap; }
		Microsoft::WRL::ComPtr<IDXGISwapChain3> GetSwapChain() { return mSwapChain; }
		Microsoft::WRL::ComPtr<ID3D12Resource> GetRenderTargetResource(int idx) { return mRenderTargets[idx]; }
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRTVHeap() { return mRtvHeap; }
		Microsoft::WRL::ComPtr<ID3D12RootSignature>  GetRootSignature() { return mRootSignature; }

		//imgui
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return mCommandList; }
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRnderTargetDescriptorHandle(int idx) { return mRenderTragetDesciptorHandle[idx]; }
		UINT GetFrameIndex() const { return mFrameIndex; }

	private:
		bool mbUseWarpDevice;

		Microsoft::WRL::ComPtr<ID3D12Device>			mDevice;
		Microsoft::WRL::ComPtr<IDXGIFactory4>			mFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain3>			mSwapChain;

		Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> mPipelineState;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue>		mCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

		// frameBuffer
		UINT mFrameIndex;
		UINT mRtvDescriptorSize;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
		FrameContext mFrameContext[2]; //double buffering
		Microsoft::WRL::ComPtr<ID3D12Resource> mRenderTargets[2]; // Double buffering
		CD3DX12_CPU_DESCRIPTOR_HANDLE mRenderTragetDesciptorHandle[2];

		// fence
		Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
		UINT64 mFenceLastSignalValue;
		HANDLE mFenceEvent;

		//imgui
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap;

		// scene view render target
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSceneRtvHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource>       mSceneResource;
		D3D12_CPU_DESCRIPTOR_HANDLE                  mSceneSRVCpu;
		D3D12_GPU_DESCRIPTOR_HANDLE                  mSceneSRVGpu;
		UINT                                         mSceneWidth;
		UINT                                         mSceneHeight;
	};

	inline GraphicDevice_DX12*& GetDevice()
	{
		static GraphicDevice_DX12* device = nullptr;
		return device;
	}
}
