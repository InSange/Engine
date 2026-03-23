#pragma once
#include "Component/Component/NComponent.h"
#include "Resource/Texture/NTexture.h"
#include "../../Resource/Material/NMaterial.h"
#include "../../Resource/Mesh/NMesh.h"

namespace NuNu
{
	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer();
		virtual ~SpriteRenderer();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

		[[noreturn]] void SetSprite(graphics::Texture* sprite) { mSprite = sprite; }
		[[noreturn]] void SetMaterial(Material* material) { mMaterial = material; }

	private:
		graphics::Texture* mSprite;
		Material* mMaterial;
		Mesh* mMesh;
	};
}


