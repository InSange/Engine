#pragma once
#include "Component/Camera/NCamera.h"
#include "Scene/NScene.h"
#include "Graphics/GraphicDevice/NGraphicDevice_DX12.h"
#include "../../Graphics/GpuBuffer/NConstantBuffer.h"
#include "../../Graphics/RenderTarget/NRenderTarget.h"

using namespace NuNu::math;
using namespace NuNu::graphics;

namespace NuNu::renderer
{
	extern Camera* mainCamera;
	extern GameObject* selectedObject;
	extern ConstantBuffer* constantBuffers[static_cast<UINT>(eCBType::End)];
	extern D3D12_GPU_DESCRIPTOR_HANDLE barbarianTexSRV;
	extern D3D12_GPU_DESCRIPTOR_HANDLE platformerTexSRV;

#if 0
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[static_cast<UINT>(eSamplerType::End)];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[static_cast<UINT>(eRasterizerState::End)];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[static_cast<UINT>(eBlendState::End)];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[static_cast<UINT>(eDepthStencilState::End)];
#endif

	extern RenderTarget* FrameBuffer;

	void Initialize();
	void RenderSceneFromCamera(Scene* scene, Camera* camera);
	void CollectRenderables(const Scene* scene, std::vector<GameObject*>& opaqueList, std::vector<GameObject*>& cutoutList
		, std::vector<GameObject*>& transparentList);

	void SortByDistance(std::vector<GameObject*>& renderList, const Vector3& cameraPos, bool bAscending);
	void RenderRenderables(const std::vector<GameObject*>& renderList, const Matrix& view, const Matrix& projection);
	void Release();
}



