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

        bool is3D = (fileName.find(L"Mesh3D") != std::wstring::npos);

        D3D12_INPUT_ELEMENT_DESC inputElementDescs2D[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };
        D3D12_INPUT_ELEMENT_DESC inputElementDescs3D[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        auto rootSignature   = GetDevice()->GetRootSignature();
        auto rootSignature3D = GetDevice()->GetRootSignature3D();

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout     = is3D
            ? D3D12_INPUT_LAYOUT_DESC{ inputElementDescs3D, _countof(inputElementDescs3D) }
            : D3D12_INPUT_LAYOUT_DESC{ inputElementDescs2D, _countof(inputElementDescs2D) };
        psoDesc.pRootSignature  = is3D ? rootSignature3D.Get() : rootSignature.Get();
        psoDesc.VS              = CD3DX12_SHADER_BYTECODE(mVSBlob.Get());
        psoDesc.PS              = CD3DX12_SHADER_BYTECODE(mPSBlob.Get());
        CD3DX12_RASTERIZER_DESC rastDesc(D3D12_DEFAULT);
        if (is3D)
            rastDesc.CullMode = D3D12_CULL_MODE_NONE;
        psoDesc.RasterizerState = rastDesc;
        psoDesc.BlendState      = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        if (is3D)
        {
            psoDesc.DepthStencilState.DepthEnable    = TRUE;
            psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
            psoDesc.DepthStencilState.DepthFunc      = D3D12_COMPARISON_FUNC_LESS_EQUAL;
            psoDesc.DepthStencilState.StencilEnable  = FALSE;
            psoDesc.DSVFormat                        = DXGI_FORMAT_D32_FLOAT;
        }
        else
        {
            psoDesc.DepthStencilState.DepthEnable   = FALSE;
            psoDesc.DepthStencilState.StencilEnable = FALSE;
        }
        psoDesc.SampleMask              = UINT_MAX;
        psoDesc.PrimitiveTopologyType   = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets        = 1;
        psoDesc.RTVFormats[0]           = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count        = 1;

        if (is3D)
            GetDevice()->CreatePipelineState(&psoDesc, mPipelineState.GetAddressOf());
        else
            GetDevice()->CreateGraphicsPipelineState(&psoDesc);

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
        if (!GetDevice()->CreateVertexShader(fileName, mVSBlob.GetAddressOf()))

        return true;
    }

    bool graphics::Shader::CreatePixelShader(const std::wstring& fileName)
    {
        if (!GetDevice()->CreatePixelShader(fileName, mPSBlob.GetAddressOf()))
            return false;

        return true;
    }

    void graphics::Shader::Bind()
    {
        if (mPipelineState)
            GetDevice()->GetCommandList()->SetPipelineState(mPipelineState.Get());

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
