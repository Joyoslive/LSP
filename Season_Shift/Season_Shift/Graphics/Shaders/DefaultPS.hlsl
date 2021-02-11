struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 nor : NORMAL;
    
};



SamplerState g_defSamp : register(s0);

Texture2D g_difTex : register(t0);
Texture2D g_specTex : register(t1);
Texture2D g_norTex : register(t2);

float4 main(VS_OUT input) : SV_TARGET
{

    float4 finalColor = float4(0.0, 0.0, 0.0, 0.0);
    
	//return float4(input.uv, 0.0f, 1.0f);
    finalColor = g_difTex.Sample(g_defSamp, input.uv);
    
    finalColor = pow(finalColor, float4(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f, 1.f));
    
    return finalColor;
}