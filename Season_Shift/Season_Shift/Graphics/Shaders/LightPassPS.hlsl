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

cbuffer camBuf : register(b1)
{
	float3 g_camPos;
};

float4 calcDirLight(float3 normal, float3 worldPos, float4 texColor)
{
	float incidentIntensity = saturate(dot(normal, normalize(-g_dlDirection)));
	float4 amb = g_dlColor * g_dlAmbIntensity;
	float4 dif = saturate(g_dlColor * incidentIntensity);

	float3 camDir = g_camPos - worldPos;
	float3 halfWay = normalize(camDir + g_dlDirection);
	float4 spec = g_dlColor * pow(max(dot(normal, halfWay), 0.0), 32.0);
	
	return (amb + amb + spec) * texColor;
}

float4 main(PS_IN input) : SV_TARGET
{
	float4 finalColor = g_difTex.Sample(g_sampler, input.uv);
	float3 normal = g_norTex.Sample(g_sampler, input.uv);
	float3 worldPos = g_posTex.Sample(g_sampler, input.uv);

	if (!(finalColor.w > -1.1 && finalColor.w < -0.9))
		finalColor = calcDirLight(normal, worldPos, finalColor);
	
	
	finalColor = pow(finalColor, float4(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f, 1.f));
	return finalColor;
}