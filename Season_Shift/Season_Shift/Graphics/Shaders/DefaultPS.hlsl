struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 nor : NORMAL;
    
};

cbuffer dirLight : register(b0)
{
    float3 g_dlDirection;
    float g_dlIntensity;
    float4 g_dlAmbColor;
    float4 g_dlDifColor;
};


SamplerState g_defSamp : register(s0);

Texture2D g_difTex : register(t0);
Texture2D g_specTex : register(t1);
Texture2D g_norTex : register(t2);

float4 calculateDirectionalLight(float3 normal, float4 textureColor)
{
    float intensity = saturate(dot(normal, normalize(-g_dlDirection)));
    float4 amb = saturate(textureColor * g_dlAmbColor);
    float4 dif = saturate(textureColor * g_dlDifColor * intensity);
    //float4 spec

    return saturate(amb + dif /*+ spec*/);
}

float4 main(VS_OUT input) : SV_TARGET
{

    float4 finalColor = float4(0.0, 0.0, 0.0, 0.0);
    
	//return float4(input.uv, 0.0f, 1.0f);
    //finalColor = g_difTex.Sample(g_defSamp, input.uv);
    finalColor = calculateDirectionalLight(normalize(input.nor), g_difTex.Sample(g_defSamp, input.uv));
    
    //Gamma correction
    finalColor = pow(finalColor, float4(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f, 1.f));
    
    return finalColor;
}