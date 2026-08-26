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

		HRESULT Save(const std::wstring& path) override;
		HRESULT Load(const std::wstring& path) override;

		void Bind();
		void BindShader();
		void BindTextures();
		void SetRenderingMode(const graphics::eRenderingMode mode);

		void SetShader(graphics::Shader* shader) { mShader = shader; }
		void SetAlbedoTexture(graphics::Texture* texture) { mAlbedoTexture = texture; mData.albedo = texture->GetName(); }

		graphics::eRenderingMode GetRenderingMode() const { return mMode; }

	private:
		graphics::eRenderingMode mMode;
		Data mData;

		graphics::Texture* mAlbedoTexture;
		graphics::Shader* mShader;
	};
}
