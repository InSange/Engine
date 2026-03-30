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
		mMesh3D->Bind();

		graphics::GetDevice()->DrawIndexedInstanced(mMesh3D->GetIndexCount(), 1, 0, 0, 0);
	}
}
