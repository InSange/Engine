#pragma once
#include <DirectXTex.h>
#include <DirectXTex.inl>


#include "Resource/NResource.h"
#include "../../Graphics/GraphicDevice/NGraphicDevice_DX12.h"

namespace NuNu::graphics
{
	class Texture : public Resource
	{
	public:
		Texture();
		virtual ~Texture();

		HRESULT Save(const std::wstring& path) override;
		HRESULT Load(const std::wstring& path) override;

		bool Create(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlag);
		bool CreateSRV();
		bool CreateUAV();
		bool CreateRTV();
		bool CreateDSV();
		bool CreateGpuView(UINT flag);
		void Bind(eShaderStage stage, UINT startSlot);

	private:
		ScratchImage mImage;
	};
}


