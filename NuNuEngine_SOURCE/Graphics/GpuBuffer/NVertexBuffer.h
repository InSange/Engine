#pragma once
#include "Graphics/GraphicDevice/NGraphicDevice_DX11.h"

namespace NuNu::graphics
{
	class VertexBuffer : public GpuBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		bool Create(const std::vector<Vertex>& vertexes);
		void Bind();

	private:
	};
}

