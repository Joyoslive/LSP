struct VS_IN
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_OUT main( VS_IN input )
{
	VS_OUT output;
	output.position = float4(input.position, 1);
	output.uv = input.uv;
	return output;

}