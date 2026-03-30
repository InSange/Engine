#pragma once
#include "High Level Interface/Renderer/NBaseRenderer.h"
#include "Resource/Mesh3D/NMesh3D.h"
#include "Resource/Graphics/Shader/NShader.h"

namespace NuNu
{
	class MeshRenderer : public BaseRenderer
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		void Initialize() override;
		void Render(const Matrix& view, const Matrix& projection) override;

		void SetMesh3D(Mesh3D* mesh)                        { mMesh3D = mesh; }
		void SetShader3D(graphics::Shader* shader)          { mShader3D = shader; }
		void SetTextureSRV(D3D12_GPU_DESCRIPTOR_HANDLE srv) { mTextureSRVGpu = srv; }

	private:
		Mesh3D*                     mMesh3D;
		graphics::Shader*           mShader3D;
		D3D12_GPU_DESCRIPTOR_HANDLE mTextureSRVGpu;
	};
}
