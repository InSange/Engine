#include "NMesh3D.h"
#include "../../Graphics/GraphicDevice/NGraphicDevice_DX12.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace NuNu
{
	Mesh3D::Mesh3D()
		: Resource(enums::eResourceType::Mesh3D)
		, mVertexBufferView({})
	{
	}

	Mesh3D::~Mesh3D()
	{
	}

	HRESULT Mesh3D::Save(const std::wstring& path)
	{
		return S_OK;
	}

	HRESULT Mesh3D::Load(const std::wstring& path)
	{
		int len = WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string narrowPath(len - 1, '\0');
		WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, narrowPath.data(), len, nullptr, nullptr);

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(narrowPath,
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
			return E_FAIL;

		std::vector<Vertex3D> vertices;
		std::vector<UINT> indices;

		for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
		{
			aiMesh* mesh = scene->mMeshes[m];
			UINT baseIndex = (UINT)vertices.size();

			for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
			{
				Vertex3D v = {};
				v.pos    = { mesh->mVertices[i].x,  mesh->mVertices[i].y,  mesh->mVertices[i].z };
				if (mesh->HasNormals())
					v.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
				if (mesh->mTextureCoords[0])
					v.uv = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
				vertices.push_back(v);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
			{
				const aiFace& face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; ++j)
					indices.push_back(baseIndex + face.mIndices[j]);
			}
		}

		if (!createVB(vertices)) return E_FAIL;
		if (!mIB.Create(indices)) return E_FAIL;

		return S_OK;
	}

	bool Mesh3D::createVB(const std::vector<Vertex3D>& vertices)
	{
		const UINT bufferSize = (UINT)vertices.size() * sizeof(Vertex3D);

		CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

		graphics::GetDevice()->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mVertexBuffer));

		UINT8* pData = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pData));
		memcpy(pData, vertices.data(), bufferSize);
		mVertexBuffer->Unmap(0, nullptr);

		mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
		mVertexBufferView.StrideInBytes  = sizeof(Vertex3D);
		mVertexBufferView.SizeInBytes    = bufferSize;

		return true;
	}

	void Mesh3D::Bind()
	{
		graphics::GetDevice()->BindVertexBuffer(0, 1, &mVertexBufferView);
		mIB.Bind();
	}
}
