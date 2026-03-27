#include "NConstantBuffer.h"

namespace NuNu::graphics
{
	ConstantBuffer::ConstantBuffer(eCBType type)
		: mSize(0)
		, mType(type)
	{
	}

	ConstantBuffer::~ConstantBuffer()
	{
	}

	bool ConstantBuffer::Create(UINT size, void* data)
	{
#if 0
		mSize = size;
		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = data;

		bool success = false;
		if (data == nullptr)
			success = GetDevice<graphics::GraphicDevice_DX11>()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
		else
			success = GetDevice<graphics::GraphicDevice_DX11>()->CreateBuffer(&desc, &sub, buffer.GetAddressOf());

		if (!success)
			assert(false && "Create constant buffer failed!");
#endif
		return true;
	}

	void ConstantBuffer::SetData(void* data) const
	{
#if 0
		GetDevice<graphics::GraphicDevice_DX11>()->SetDataGpuBuffer(buffer.Get(), data, mSize);
#endif
	}

	void ConstantBuffer::Bind(eShaderStage stage) const
	{
#if 0
		GetDevice<graphics::GraphicDevice_DX11>()->BindConstantBuffer(stage, mType, buffer.Get());
#endif
	}
}
