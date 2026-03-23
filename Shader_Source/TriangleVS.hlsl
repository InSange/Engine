#include "ConstantBuffers.hlsli"

struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
    //float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 color : COLOR;
    //float2 uv : TEXCOORD;
};

// 버텍스 셰이더
VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0.0f;
    
    float4 pos = mul(float4(input.pos, 1.0f), WorldMatrix);
    float4 viewPos = mul(pos, ViewMatrix);
    float4 projPos = mul(viewPos, ProjectionMatrix);
    
    output.pos = projPos;
    output.color = input.color;

    return output;
}

/*
Vertex Shader
버텍스 셰이더(Vertex Shader)는 그래픽스 파이프라인에서 각 정점(Vertex)의 위치를 처리하는 GPU 프로그램으로, 주로 모델의 변환(예: 회전, 이동, 스케일)을 계산하고, 정점 정보를 변형하여 월드 좌표계에서 화면 좌표계로 변환하는 역할을 하게 된다.

변환(Matrix Transformations) : 오브젝트의 위치, 회전, 스케일을 처리하여, 모델 좌표계에서 뷰, 클립 좌표계로 변환한다. 이는 주로 모델-뷰-프로젝션 행렬을 곱하는 과정에서 이루어진다.
정점 속성 전달 : 각 정점의 위치뿐 아니라, 정점에 대한 추가적인 속성(예 : 색상, 텍스처 좌표, 노멀 벡터 등)을 픽셀 셰이더로 전달하게 된다. 이를 통해 픽셀 셰이더는 각 픽셀에 필요한 정보를 기반으로 최종 색을 계산할 수 있다.
조명 계산 : 버텍스 셰이더에서 조명 계산을 일부 수행할 수 있다. 예를 들어, 고전적인 램버트 조명 모델이나 퐁 조명 모델 등 기본적인 조명 효과는 버텍스 셰이더에서 계산할 수 있다(다만, 버텍스 기반 계산이므로 버텍스가 충분히 많지 않을때 엣지가 두드러지게 보이는 증상이 있다)
정점 변형 : GPU는 버텍스 셰이더를 사용해 정점들을 변형할 수 있는데 이를 활용해 정점의 움직임을 제어해 다양한 움직임을 만들수 있다.
텍스처 좌표 계산 : 버텍스 셰이더는 텍스처 좌표를 계산하고, 이를 픽셀 셰이더로 전달한다. 예를 들어, 정점의 위치에 따라 텍스처 맵핑이 다르게 적용될 수 있다. 또한, 이를 활용해 텍스처 UV 정보값을 왜곡해 다양한 효과를 만들어 낼 수도 있다

Vertex Shader의 동작 과정

입력(Input) : CPU에서 정점 데이터(예: 위치, 색상, 텍스처 좌표 등)를 GPU로 전달한다. 이 데이터는 버텍스 셰이더에서 변환되게 된다(Input Assembly)
계산(Processing) : 정점의 변환, 조명, 텍스처 좌표 등의 계산을 수행.
출력(Output) : 최종적으로 변환된 정점 데이터를 그래픽스 파이프라인의 다음 단계로 보간기(Interpolator)를 통해 전달한다(주로 픽셀 셰이더)

 
주요 함수 및 구성

float4 : 위치, 색상 등의 데이터형으로 주로 4D 벡터로 나타내는 자료형.
mul : 행렬 변환을 위해 많이 사용되는 함수로, 버텍스 위치에 변환 행렬을 곱하는 데 사용.
출처: https://illu.tistory.com/576 [illustration of life:티스토리]
*/