#include "NMeshRenderer.h"
#include "Component/Transform/NTransform.h"
#include "GameObject/NGameObject.h"
#include "Graphics/GraphicDevice/NGraphicDevice_DX12.h"

namespace NuNu
{
	MeshRenderer::MeshRenderer()
		: BaseRenderer(eComponentType::MeshRenderer)
		, mMesh3D(nullptr)
		, mShader3D(nullptr)
		, mTextureSRVGpu{}
		, mTintColor{ 1.0f, 1.0f, 1.0f, 1.0f }
	{
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
