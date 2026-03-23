#include "NShader.h"
#include "../../../Renderer/NRenderer.h"
#include "../../NResources.h"

namespace NuNu::graphics
{
    bool Shader::bWireframe = true;

    graphics::Shader::Shader()
        : Resource(eResourceType::Shader)
        , mRasterizerState(eRasterizerState::SolidBack)
        , mBlendState(eBlendState::AlphaBlend)
        , mDepthStencilState(eDepthStencilState::LessEqual)
    {

    }

    graphics::Shader::~Shader()
    {
    }

    HRESULT Shader::Save(const std::wstring& path)
    {
        return E_NOTIMPL;
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
        if (bWireframe)
        {
            Shader* wireframeShader = Resources::Find<Shader>(L"WireframeShader");
            Microsoft::WRL::ComPtr<ID3D11VertexShader> wireframeShaderVS = wireframeShader->GetVS();
            Microsoft::WRL::ComPtr<ID3D11PixelShader> wireframeShaderPS = wireframeShader->GetPS();
            Microsoft::WRL::ComPtr<ID3D11RasterizerState> wireframeRasterizerState
                = renderer::rasterizerStates[static_cast<UINT>(eRasterizerState::WireFrame)];

            GetDevice()->BindVS(wireframeShaderVS.Get());
            GetDevice()->BindPS(wireframeShaderPS.Get());
            GetDevice()->BindRasterizerState(wireframeRasterizerState.Get());
            GetDevice()->BindBlendState(renderer::blendStates[static_cast<UINT>(mBlendState)].Get(), nullptr, 0xffffff);
            GetDevice()->BindDepthStencilState(renderer::depthStencilStates[static_cast<UINT>(mDepthStencilState)].Get(), 0);

            return;
        }

        if (mVS)
            GetDevice()->BindVS(mVS.Get());
        if (mPS)
            GetDevice()->BindPS(mPS.Get());

        GetDevice()->BindRasterizerState(renderer::rasterizerStates[static_cast<UINT>(mRasterizerState)].Get());
        GetDevice()->BindBlendState(renderer::blendStates[static_cast<UINT>(mBlendState)].Get(), nullptr, 0xffffff);
        GetDevice()->BindDepthStencilState(renderer::depthStencilStates[static_cast<UINT>(mDepthStencilState)].Get(), 0);
    }
}
