#include "Component/SpriteRenderer/NSpriteRenderer.h"
#include "GameObject/NGameObject.h"
#include "../../High Level Interface/Renderer/NRenderer.h"
#include "../../Resource/NResources.h"
#include "../../Resource/Texture/NTexture.h"

namespace NuNu
{
	SpriteRenderer::SpriteRenderer()
		: BaseRenderer(eComponentType::SpriteRenderer)
	{
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}

	void SpriteRenderer::Initialize()
	{
		BaseRenderer::Initialize();

		Mesh* mesh = Resources::Find<Mesh>(L"RectMesh");
		Material* material = Resources::Find<Material>(L"TriangleMaterial");

		SetMesh(mesh);
		SetMaterial(material);
	}

	void SpriteRenderer::Update()
	{
		BaseRenderer::Update();
	}

	void SpriteRenderer::LateUpdate()
	{
		BaseRenderer::LateUpdate();
	}

	void SpriteRenderer::Render(const Matrix& view, const Matrix& projection)
	{
		BaseRenderer::Render(view, projection);

		if (mSprite)
			mSprite->Bind(eShaderStage::PS, CAST_UINT(eTextureType::Sprite));

		BaseRenderer::Draw();
	}
}

