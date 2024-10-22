struct VS_OUT
{
    float4 pos : SV_Position;
    float3 localPos : POSITION;
};

struct PS_OUT
{
    float4 positionWS : SV_TARGET0;
    float4 positionNormal : SV_TARGET1;
    float4 uv : SV_TARGET2;
    float4 diffuseColor : SV_TARGET3;
};

TextureCube g_texCube : register(t5);
SamplerState g_skySampler: register(s5);

PS_OUT main(VS_OUT input)
{
    PS_OUT output = (PS_OUT)0;

    output.positionWS = float4(0.0, 0.0, 0.0, 0.0);
    output.positionNormal = float4(0.0, 0.0, 0.0, 0.0);
    output.uv = float4(0.0, 0.0, 0.0, 0.0);
    output.diffuseColor = float4(g_texCube.Sample(g_skySampler, input.localPos).xyz, -1.0);

    return output;
}