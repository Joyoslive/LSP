SamplerState g_sampler : register(s0);
Texture2D g_bbTex : register(t0);

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
	return g_bbTex.Sample(g_sampler, input.uv);
}