Texture2D g_posTex : register(t0);
Texture2D g_norTex : register(t1);
Texture2D g_uvTex : register(t2);
Texture2D g_difTex : register(t3);

SamplerState g_sampler : register(s0);

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
	float4 finalColor = g_difTex.Sample(g_sampler, input.uv);
	finalColor = pow(finalColor, float4(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f, 1.f));
	return finalColor;
}