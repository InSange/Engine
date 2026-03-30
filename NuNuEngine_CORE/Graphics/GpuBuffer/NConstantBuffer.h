#pragma once
#include "../GraphicDevice/NGraphicDevice_DX12.h"

namespace NuNu::graphics
{
	class ConstantBuffer : public GpuBuffer
	{
	public:
		ConstantBuffer(eCBType type);
		virtual ~ConstantBuffer();

		bool Create(UINT size, void* data = nullptr);

		void SetData(void* data) const;
		void Bind(eShaderStage stage) const;

	private:
		UINT mSize;
		eCBType mType;
		mutable void* mData;
	};
}

