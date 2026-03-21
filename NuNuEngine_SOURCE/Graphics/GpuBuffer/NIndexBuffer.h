#pragma once
#include "../GraphicDevice/NGraphicDevice_DX11.h"

namespace NuNu::graphics
{
	class IndexBuffer : public GpuBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		bool Create(const std::vector<UINT>& indices);
		void Bind() const;

	private:
	};
}
