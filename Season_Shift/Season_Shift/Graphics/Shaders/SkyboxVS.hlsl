static const float3 CUBE[36] =
{
float3(0.5f, -0.5f, -0.5f),
float3(-0.5f, -0.5f, -0.5f),
float3(-0.5f, 0.5f, -0.5f),

float3(-0.5f, 0.5f, -0.5f),
float3(0.5f, 0.5f, -0.5f),
float3(0.5f, -0.5f, -0.5f),

float3(-0.5f, -0.5f, 0.5f),
float3(0.5f, -0.5f, 0.5f),
float3(0.5f, 0.5f, 0.5f),

float3(0.5f, 0.5f, 0.5f),
float3(-0.5f, 0.5f, 0.5f),
float3(-0.5f, -0.5f, 0.5f),

float3(-0.5f, -0.5f, -0.5f),
float3(-0.5f, -0.5f, 0.5f),
float3(-0.5f, 0.5f, 0.5f),
    
float3(-0.5f, 0.5f, 0.5f),
float3(-0.5f, 0.5f, -0.5f),
float3(-0.5f, -0.5f, -0.5f),

float3(0.5f, -0.5f, 0.5f),
float3(0.5f, -0.5f, -0.5f),
float3(0.5f, 0.5f, -0.5f),

float3(0.5f, 0.5f, -0.5f),
float3(0.5f, 0.5f, 0.5f),
float3(0.5f, -0.5f, 0.5f),

float3(0.5f, 0.5f, -0.5f),
float3(-0.5f, 0.5f, -0.5f),
float3(-0.5f, 0.5f, 0.5f),

float3(-0.5f, 0.5f, 0.5f),
float3(0.5f, 0.5f, 0.5f),
float3(0.5f, 0.5f, -0.5f),

float3(0.5f, -0.5f, 0.5f),
float3(-0.5f, -0.5f, 0.5f),
float3(-0.5f, -0.5f, -0.5f),
    
float3(-0.5f, -0.5f, -0.5f),
float3(0.5f, -0.5f, -0.5f),
float3(0.5f, -0.5f, 0.5f)
};

cbuffer VPMat : register(b5)
{
    matrix g_viewMat;
    matrix g_projMat;
    matrix g_rotMat;
}

struct VS_OUT
{
    float4 pos : SV_Position;
    float3 localPos : POSITION;
};

VS_OUT main(uint vID : SV_VertexID)
{
    VS_OUT output = (VS_OUT)0;
    //output.localPos = CUBE[vID];                    // This works as the "UV" for the cube. We make sure the vertices have 
    output.localPos = mul(g_rotMat, float4(normalize(CUBE[vID]), 0.f)).xyz;

    output.pos = mul(g_viewMat, float4(CUBE[vID], 0.f));        // Make sure to take camera rotation into account (look at correct side of cube)
    output.pos = mul(g_projMat, output.pos);                    // Take perspective into account
    output.pos.z = output.pos.w;                                // Place with furthest distance

	return output;
}