struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 nor : NORMAL;
    
};

float4 main(VS_OUT input) : SV_TARGET
{
	return float4(input.uv, 1.0f, 1.0f);
}