cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

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
    
    //output.pos = float4(input.pos, 1.0);
    matrix wvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    output.pos = mul(wvp, float4(input.pos, 1));
    output.uv = input.uv;
    output.nor = normalize(input.nor);
    
	return output;
}