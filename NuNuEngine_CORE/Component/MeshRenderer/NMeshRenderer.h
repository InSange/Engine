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

		const char* GetComponentKey() const override { return "MeshRenderer"; }
		void Initialize() override;
		void Render(const Matrix& view, const Matrix& projection) override;

		void SetMesh3D(Mesh3D* mesh)                        { mMesh3D = mesh; }
		void SetShader3D(graphics::Shader* shader)          { mShader3D = shader; }
		void SetTextureSRV(D3D12_GPU_DESCRIPTOR_HANDLE srv) { mTextureSRVGpu = srv; }
		void SetColor(float r, float g, float b, float a = 1.0f) { mTintColor[0]=r; mTintColor[1]=g; mTintColor[2]=b; mTintColor[3]=a; }
		const float* GetColor() const { return mTintColor; }

		// 직렬화용 이름 getter
		std::string GetMesh3DName()  const;
		std::string GetShaderName()  const;
		std::string GetTextureName() const { return mTextureName; }
		void SetTextureName(const std::string& name) { mTextureName = name; }

	private:
		Mesh3D*                     mMesh3D;
		graphics::Shader*           mShader3D;
		D3D12_GPU_DESCRIPTOR_HANDLE mTextureSRVGpu;
		float                       mTintColor[4];
		std::string                 mTextureName;  // 직렬화용 ("barbarian", "platformer", "")
	};
}
