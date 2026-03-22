#pragma once
#include "Component/Camera/NCamera.h"
#include "Graphics/GraphicDevice/NGraphicDevice_DX11.h"
#include "../Resource/Mesh/NMesh.h"
#include "../Graphics/GpuBuffer/NConstantBuffer.h"

using namespace NuNu::math;
using namespace NuNu::graphics;

namespace NuNu::renderer
{
	extern Camera* mainCamera;

	extern Mesh* mesh;

	extern graphics::ConstantBuffer constantBuffers[(UINT)eCBType::End];

	extern ID3D11Buffer* constantBuffer;
	extern ID3D11InputLayout* inputLayouts;

	void Initialize();
	void Release();
}



