#include "NMeshRenderer.h"
#include "Component/Transform/NTransform.h"
#include "GameObject/NGameObject.h"
#include "Graphics/GraphicDevice/NGraphicDevice_DX12.h"
#include "Resource/Mesh3D/NMesh3D.h"
#include "Resource/Graphics/Shader/NShader.h"

namespace NuNu
{
	MeshRenderer::MeshRenderer()
		: BaseRenderer(eComponentType::MeshRenderer)
		, mMesh3D(nullptr)
		, mShader3D(nullptr)
		, mTextureSRVGpu{}
		, mTintColor{ 1.0f, 1.0f, 1.0f, 1.0f }
		, mTextureName("")
	{
	}

	std::string MeshRenderer::GetMesh3DName() const
	{
		if (mMesh3D == nullptr) return "";
		int n = WideCharToMultiByte(CP_UTF8, 0, mMesh3D->GetName().c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string s(n - 1, 0);
		WideCharToMultiByte(CP_UTF8, 0, mMesh3D->GetName().c_str(), -1, s.data(), n, nullptr, nullptr);
		return s;
	}

	std::string MeshRenderer::GetShaderName() const
	{
		if (mShader3D == nullptr) return "";
		int n = WideCharToMultiByte(CP_UTF8, 0, mShader3D->GetName().c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string s(n - 1, 0);
		WideCharToMultiByte(CP_UTF8, 0, mShader3D->GetName().c_str(), -1, s.data(), n, nullptr, nullptr);
		return s;
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::Initialize()
	{
	}

	void MeshRenderer::Render(const Matrix& view, const Matrix& projection)
	{
		if (mMesh3D == nullptr || mShader3D == nullptr)
			return;

		Transform* tr = GetOwner()->GetComponent<Transform>();
		if (tr)
			tr->Bind(view, projection);

		mShader3D->Bind();

		if (mTextureSRVGpu.ptr != 0)
			graphics::GetDevice()->GetCommandList()->SetGraphicsRootDescriptorTable(1, mTextureSRVGpu);

		graphics::GetDevice()->GetCommandList()->SetGraphicsRoot32BitConstants(2, 4, mTintColor, 0);

		mMesh3D->Bind();

		graphics::GetDevice()->DrawIndexedInstanced(mMesh3D->GetIndexCount(), 1, 0, 0, 0);
	}
}
