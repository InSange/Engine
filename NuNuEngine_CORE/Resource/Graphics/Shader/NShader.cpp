#include "NShader.h"
#include "../../../High Level Interface/Renderer/NRenderer.h"
#include "../../NResources.h"

namespace NuNu::graphics
{
    bool Shader::bWireframe = false;

    graphics::Shader::Shader()
        : Resource(eResourceType::Shader)
        , mRasterizerState(eRasterizerState::SolidNone)
        , mBlendState(eBlendState::Opaque)
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
#if 0
        if (!GetDevice<GraphicDevice_DX11>()->CreateVertexShader(fileName, mVSBlob.GetAddressOf(), mVS.GetAddressOf()))
            return false;
#endif
        return true;
    }

    bool graphics::Shader::CreatePixelShader(const std::wstring& fileName)
    {
#if 0
        if (!GetDevice<GraphicDevice_DX11>()->CreatePixelShader(fileName, mPSBlob.GetAddressOf(), mPS.GetAddressOf()))
            return false;
#endif
        return true;
    }

    void graphics::Shader::Bind()
    {
#if 0
        if (bWireframe)
        {
            Shader* wireframeShader = Resources::Find<Shader>(L"WireframeShader");
            Microsoft::WRL::ComPtr<ID3D11VertexShader> wireframeShaderVS = wireframeShader->GetVS();
            Microsoft::WRL::ComPtr<ID3D11PixelShader> wireframeShaderPS = wireframeShader->GetPS();
            Microsoft::WRL::ComPtr<ID3D11RasterizerState> wireframeRasterizerState
                = renderer::rasterizerStates[static_cast<UINT>(eRasterizerState::WireFrame)];

            GetDevice<GraphicDevice_DX11>()->BindVS(wireframeShaderVS.Get());
            GetDevice<GraphicDevice_DX11>()->BindPS(wireframeShaderPS.Get());
            GetDevice<GraphicDevice_DX11>()->BindRasterizerState(wireframeRasterizerState.Get());
            GetDevice<GraphicDevice_DX11>()->BindBlendState(renderer::blendStates[static_cast<UINT>(mBlendState)].Get(), nullptr, 0xffffff);
            GetDevice<GraphicDevice_DX11>()->BindDepthStencilState(renderer::depthStencilStates[static_cast<UINT>(mDepthStencilState)].Get(), 0);

            return;
        }

        if (mVS)
            GetDevice<GraphicDevice_DX11>()->BindVS(mVS.Get());
        if (mPS)
            GetDevice<GraphicDevice_DX11>()->BindPS(mPS.Get());

        GetDevice<GraphicDevice_DX11>()->BindRasterizerState(renderer::rasterizerStates[static_cast<UINT>(mRasterizerState)].Get());
        GetDevice<GraphicDevice_DX11>()->BindBlendState(renderer::blendStates[static_cast<UINT>(mBlendState)].Get(), nullptr, 0xffffff);
        GetDevice<GraphicDevice_DX11>()->BindDepthStencilState(renderer::depthStencilStates[static_cast<UINT>(mDepthStencilState)].Get(), 0);
#endif
    }
}
