#include "Resource/Graphics/Shader/NShader.h"
#include "NShader.h"

namespace NuNu::graphics
{
    graphics::Shader::Shader()
        : Resource(enums::eResourceType::Shader)
    {

    }

    graphics::Shader::~Shader()
    {
    }

    HRESULT graphics::Shader::Load(const std::wstring& path)
    {
        size_t fineNameBeginOffset = path.rfind(L"\\") + 1;
        size_t fineNameEndOffset = path.length() - fineNameBeginOffset;

        const std::wstring fileName(path.substr(fineNameBeginOffset, fineNameEndOffset));

        if (!Create(eShaderStage::VS, fileName)) return S_FALSE;
        if (!Create(eShaderStage::PS, fileName)) return S_FALSE;

        return S_OK;
    }

    bool graphics::Shader::Create(const eShaderStage stage, const std::wstring& fileName)
    {
        if (stage == eShaderStage::VS)
            CreateVertexShader(fileName);
        if (stage == eShaderStage::PS)
            CreatePixelShader(fileName);

        return true;
    }

    bool graphics::Shader::CreateVertexShader(const std::wstring& fileName)
    {
        if (!GetDevice()->CreateVertexShader(fileName, mVSBlob.GetAddressOf(), mVS.GetAddressOf()))
            return false;

        return true;
    }

    bool graphics::Shader::CreatePixelShader(const std::wstring& fileName)
    {
        if (!GetDevice()->CreatePixelShader(fileName, mPSBlob.GetAddressOf(), mPS.GetAddressOf()))
            return false;

        return true;
    }

    void graphics::Shader::Bind()
    {
        if (mVS)
            GetDevice()->BindVS(mVS.Get());
        if (mPS)
            GetDevice()->BindPS(mPS.Get());
    }
}