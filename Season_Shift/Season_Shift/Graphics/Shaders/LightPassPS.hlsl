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

cbuffer dirLight : register(b0)
{
	float3 g_dlDirection;
	float g_dlAmbIntensity;
	float4 g_dlColor;
};

float4 calcDirLight(float3 normal, float4 texColor)
{
	float incidentIntensity = saturate(dot(normal, normalize(-g_dlDirection)));
	float4 amb = g_dlColor * g_dlAmbIntensity;
	float4 dif = saturate(g_dlColor * incidentIntensity);
	float4 spec = float4(0, 0, 0, 0);
	return (amb + dif + spec) * texColor;
}

float4 main(PS_IN input) : SV_TARGET
{
	float4 finalColor = g_difTex.Sample(g_sampler, input.uv);
	float3 normal = g_norTex.Sample(g_sampler, input.uv);
	finalColor = calcDirLight(normal, finalColor);
	finalColor = pow(finalColor, float4(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f, 1.f));
	return finalColor;
}