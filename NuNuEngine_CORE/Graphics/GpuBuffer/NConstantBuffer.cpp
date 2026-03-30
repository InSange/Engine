#include "NConstantBuffer.h"

namespace NuNu::graphics
{
	ConstantBuffer::ConstantBuffer(eCBType type)
		: mSize(0)
		, mType(type)
		, mData(nullptr)
	{
	}

	ConstantBuffer::~ConstantBuffer()
	{
	}

	bool ConstantBuffer::Create(UINT size, void* data)
	{
		mSize = size;
		return true;
	}

	void ConstantBuffer::SetData(void* data) const
	{
		mData = data;
	}

	void ConstantBuffer::Bind(eShaderStage stage) const
	{
		if (mData == nullptr)
			return;

		// root parameter index = static_cast<UINT>(mType)
		// CBSLOT_TRANSFORM = 0 → root parameter 0
		UINT rootParamIndex = static_cast<UINT>(mType);
		GetDevice()->GetCommandList()->SetGraphicsRoot32BitConstants(
			rootParamIndex,
			mSize / sizeof(UINT32),
			mData,
			0);
	}
}
