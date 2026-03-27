#pragma once
#include "../GraphicDevice/NGraphicDevice_DX12.h"

namespace NuNu::graphics
{
	class IndexBuffer : public GpuBuffer
	{
	public:
		IndexBuffer();
		virtual ~IndexBuffer();

		bool Create(const std::vector<UINT>& indices);
		void Bind() const;

		UINT GetIndexCount() const { return mIndexCount; }

	private:
		UINT mIndexCount;
	};
}
