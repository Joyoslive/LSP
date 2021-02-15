struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 nor : NORMAL;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float3 worldPos : WORLDPOS;
    float2 uv : TEXCOORD;
    float3 nor : NORMAL;  
};

cbuffer matrices : register(b0)
{
    matrix g_wm, g_vm, g_pm;
    
}

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
   // output.pos = float4(input.pos, 1.0);
    
    output.pos = mul(g_pm, mul(g_vm, mul(g_wm, float4(input.pos, 1.0))));
    output.worldPos = mul(g_wm, float4(input.pos, 1.0)).xyz;
    output.uv = input.uv;
    output.nor = mul(g_wm, float4(normalize(input.nor), 0.0)).xyz;
    
	return output;
}