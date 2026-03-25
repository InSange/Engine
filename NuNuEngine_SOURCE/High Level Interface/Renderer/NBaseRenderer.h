#pragma once
#include "Component/Component/NComponent.h"
#include "Resource/Texture/NTexture.h"
#include "Resource/Material/NMaterial.h"
#include "Resource/Mesh/NMesh.h"

namespace NuNu
{
	class BaseRenderer : public Component
	{
	public:
		BaseRenderer(eComponentType type);
		virtual ~BaseRenderer();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(const Matrix& view, const Matrix& projection) override;

		void Draw();

		const Material* GetMaterial() const { return mMaterial; }
		void SetMaterial(Material* material) { mMaterial = material; }

		const Mesh* GetMesh() const { return mMesh; }
		void SetMesh(Mesh* mesh) { mMesh = mesh; }

	private:
		Material* mMaterial;
		Mesh* mMesh;
	};
}