#pragma once
#include "../NResource.h"
#include "../Texture/NTexture.h"
#include "../Graphics/Shader/NShader.h"

namespace NuNu
{
	class Material : public Resource
	{
	public:
		struct Data
		{
			std::wstring albedo;
		};

		Material();
		virtual ~Material();

		virtual HRESULT Save(const std::wstring& path) override;
		virtual HRESULT Load(const std::wstring& path) override;

		void Bind();

		void SetShader(graphics::Shader* shader) { mShader = shader; }
		void SetAlbedoTexture(graphics::Texture* texture) { mAlbedoTexture = texture; mData.albedo = texture->GetName(); }

	private:
		graphics::eRenderingMode mMode;
		Material::Data mData;

		graphics::Texture* mAlbedoTexture;
		graphics::Shader* mShader;
	};
}