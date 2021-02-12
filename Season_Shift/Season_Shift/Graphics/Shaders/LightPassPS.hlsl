Texture2D g_posTex : register(t0);
Texture2D g_norTex : register(t1);
Texture2D g_uvTex : register(t2);
Texture2D g_difTex : register(t3);

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
	return float4(input.uv, 0, 1);
}