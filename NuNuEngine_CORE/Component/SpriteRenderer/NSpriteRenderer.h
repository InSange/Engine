#pragma once
#include "Component/Component/NComponent.h"
#include "Resource/Texture/NTexture.h"
#include "../../Resource/Material/NMaterial.h"
#include "../../Resource/Mesh/NMesh.h"
#include "../../High Level Interface/Renderer/NBaseRenderer.h"

namespace NuNu
{
	class SpriteRenderer : public BaseRenderer
	{
	public:
		SpriteRenderer();
		virtual ~SpriteRenderer();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(const Matrix& view, const Matrix& projection) override;

		void SetSprite(graphics::Texture* sprite) { mSprite = sprite; }

	private:
		graphics::Texture* mSprite;
	};
}


