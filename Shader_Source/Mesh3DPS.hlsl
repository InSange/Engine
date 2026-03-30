struct PSInput
{
    float4 pos    : SV_Position;
    float3 normal : NORMAL;
    float2 uv     : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float3 lightDir = normalize(float3(0.5f, 1.0f, -0.5f));
    float  diffuse  = saturate(dot(normalize(input.normal), lightDir));
    float3 color    = float3(0.8f, 0.6f, 0.4f) * (diffuse * 0.8f + 0.2f);
    return float4(color, 1.0f);
}
