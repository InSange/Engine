#include "Samplers.hlsli"

struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

// 픽셀 셰이더
float4 main(VSOutput input) : SV_Target
{
    return input.color;
}

/*
Pixel Shader
픽셀 셰이더(Pixel Shader)는 그래픽스 파이프라인에서 각 픽셀의 색상, 명도, 텍스처 정보 등을 계산하는 GPU 프로그램으로, 주로 화면에 렌더링될 각 픽셀에 대한 최종 처리를 담당하게 된다.여기에 각 오브젝트의 픽셀당 조명 및 음영 계산(lighting & ambient color), 텍스처 맵핑, 포스트 프로세싱 등이 해당된다
출처: https://illu.tistory.com/576 [illustration of life:티스토리]
*/