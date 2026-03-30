#include "ConstantBuffers.hlsli"

struct VSInput
{
    float3 pos    : POSITION;
    float3 normal : NORMAL;
    float2 uv     : TEXCOORD;
};

struct VSOutput
{
    float4 pos    : SV_Position;
    float3 normal : NORMAL;
    float2 uv     : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput)0;

    float4 worldPos = mul(float4(input.pos, 1.0f), WorldMatrix);
    float4 viewPos  = mul(worldPos, ViewMatrix);
    output.pos      = mul(viewPos, ProjectionMatrix);
    output.normal   = mul(float4(input.normal, 0.0f), WorldMatrix).xyz;
    output.uv       = input.uv;

    return output;
}
