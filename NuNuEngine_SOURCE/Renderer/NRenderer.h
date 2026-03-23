#pragma once
#include "Component/Camera/NCamera.h"
#include "Graphics/GraphicDevice/NGraphicDevice_DX11.h"
#include "../Graphics/GpuBuffer/NConstantBuffer.h"

using namespace NuNu::math;
using namespace NuNu::graphics;

namespace NuNu::renderer
{
	extern Camera* mainCamera;

	extern graphics::ConstantBuffer* constantBuffers[(UINT)eCBType::End];

	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)eRasterizerState::End];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)eBlendState::End];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)eDepthStencilState::End];

	void Initialize();
	void Release();
}



