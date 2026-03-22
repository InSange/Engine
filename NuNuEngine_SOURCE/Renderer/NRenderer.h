#pragma once
#include "Component/Camera/NCamera.h"
#include "Graphics/GraphicDevice/NGraphicDevice_DX11.h"
#include "../Graphics/GpuBuffer/NConstantBuffer.h"

using namespace NuNu::math;
using namespace NuNu::graphics;

namespace NuNu::renderer
{
	extern Camera* mainCamera;

	extern graphics::ConstantBuffer constantBuffers[(UINT)eCBType::End];

	void Initialize();
	void Release();
}



