struct VS_OUT
{
    float4 pos : SV_Position;
    float3 worldPos : WORLDPOS;
    float2 uv : TEXCOORD;
    float3 nor : NORMAL;
};

struct PS_OUT
{
    float4 positionWS : SV_TARGET0;
    float4 positionNormal : SV_TARGET1;
    float4 uv : SV_TARGET2;
    float4 diffuseColor : SV_TARGET3;
};

SamplerState g_defSamp : register(s0);
Texture2D g_difTex : register(t0);
Texture2D g_specTex : register(t1);
Texture2D g_norTex : register(t2);

PS_OUT main(VS_OUT input)
{
    PS_OUT output = (PS_OUT)0;

    float4 diffuseColor = g_difTex.Sample(g_defSamp, input.uv);

    output.diffuseColor = diffuseColor;
    output.positionWS = float4(input.worldPos, 1.0);
    output.positionNormal = float4(normalize(input.nor), 0.0);
    output.uv = float4(input.uv, 0.0, 0.0);

    return output;
}