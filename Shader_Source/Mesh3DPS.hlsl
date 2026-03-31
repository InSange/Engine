#include "ConstantBuffers.hlsli"

Texture2D<float4> albedo : register(t0);
SamplerState      samp   : register(s0);

struct PSInput
{
    float4 pos    : SV_Position;
    float3 normal : NORMAL;
    float2 uv     : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float3 lightDir  = normalize(float3(0.5f, 1.0f, -0.5f));
    float  diffuse   = saturate(dot(normalize(input.normal), lightDir));
    float4 texColor  = albedo.Sample(samp, input.uv);
    float3 color     = texColor.rgb * TintColor.rgb * (diffuse * 0.8f + 0.2f);
    return float4(color, texColor.a * TintColor.a);
}
