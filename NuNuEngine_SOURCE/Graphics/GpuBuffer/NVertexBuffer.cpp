#include "NVertexBuffer.h"

namespace NuNu
{
	graphics::VertexBuffer::VertexBuffer()
	{
	}

	graphics::VertexBuffer::~VertexBuffer()
	{
	}

	bool graphics::VertexBuffer::Create(const std::vector<Vertex>& vertexes)
	{
		desc.ByteWidth = sizeof(Vertex) * CAST_UINT(vertexes.size());
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sub = { };
		sub.pSysMem = vertexes.data();

		if (!(GetDevice<GraphicDevice_DX11>()->CreateBuffer(&desc, &sub, buffer.GetAddressOf())))
			assert(false && "Create vertex buffer failed!");

		return true;
	}

	void graphics::VertexBuffer::Bind()
	{
		UINT offset = 0;
		UINT vertexSize = sizeof(Vertex);
		GetDevice<GraphicDevice_DX11>()->BindVertexBuffer(0, 1, buffer.GetAddressOf(), &vertexSize, &offset);
	}
}
