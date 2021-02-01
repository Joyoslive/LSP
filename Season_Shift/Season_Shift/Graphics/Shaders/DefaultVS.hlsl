struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 nor : NORMAL;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 nor : NORMAL;
    
};


VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    output.pos = float4(input.pos, 1.0);
    output.uv = input.uv;
    output.nor = normalize(input.nor);
    
	return output;
}