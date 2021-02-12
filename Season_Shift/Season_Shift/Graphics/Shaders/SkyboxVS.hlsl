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

struct VS_OUT
{
    float4 pos : SV_Position;
    float localPos : POSITION;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}