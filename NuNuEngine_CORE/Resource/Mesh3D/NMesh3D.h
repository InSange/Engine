#pragma once
#include "../NResource.h"
#include "../../Graphics/NGraphics.h"
#include "../../Graphics/GpuBuffer/NIndexBuffer.h"

namespace NuNu
{
	class Mesh3D : public Resource
	{
	public:
		struct Vertex3D
		{
			math::Vector3 pos;
			math::Vector3 normal;
			math::Vector2 uv;
		};

		Mesh3D();
		~Mesh3D();

		HRESULT Save(const std::wstring& path) override;
		HRESULT Load(const std::wstring& path) override;

		void Bind();
		UINT GetIndexCount() const { return mIB.GetIndexCount(); }

	private:
		bool createVB(const std::vector<Vertex3D>& vertices);

		Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
		graphics::IndexBuffer mIB;
	};
}
