#pragma once
#include "../NResource.h"
#include "../../Graphics/InputLayout/NInputLayout.h"
#include "../../Graphics/GpuBuffer/NVertexBuffer.h"
#include "../../Graphics/GpuBuffer/NIndexBuffer.h"

namespace NuNu
{

	class Mesh : public Resource
	{
	public:
		struct MeshData
		{
			MeshData();
			virtual ~MeshData();

			//D3D11_PRIMITIVE_TOPOLOGY mTopology;
			std::vector<graphics::Vertex> vertices;

			std::vector<UINT> indices;
		};

		Mesh();
		~Mesh();

		HRESULT Save(const std::wstring& path) override;
		HRESULT Load(const std::wstring& path) override;

		bool CreateVB(const std::vector<graphics::Vertex>& vertices);
		bool CreateIB(const std::vector<UINT>& indices);
		//void SetVertexBufferParams(UINT vertexCount, D3D11_INPUT_ELEMENT_DESC* layout, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength);

		void Bind();

		UINT GetIndexCount() const { return mIB.GetIndexCount(); }

	private:
		graphics::InputLayout mInputLayout;
		graphics::VertexBuffer mVB;
		graphics::IndexBuffer mIB;

		MeshData mData;
	};
}
