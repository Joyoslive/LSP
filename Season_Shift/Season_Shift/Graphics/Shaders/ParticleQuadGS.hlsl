
cbuffer Transforms : register(b0)
{
    matrix cameraWorldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float3 color : COLOR;
    float scale : SCALE;
    float angle : ANGLE;
};


struct GS_Out
{
	float4 pos : SV_Position;
    float4 worldPos : WORLDPOS;
    float4 color : COLOR;
};


static const float4 quadVertexArray[4] =
{
    float4(-1, 1, 0, 0),
    float4(1, 1, 0, 0),
    float4(-1, -1, 0, 0),
    float4(1, -1, 0, 0),
};


[maxvertexcount(4)]
void main(point VS_OUT input[1], inout TriangleStream< GS_Out > output)
{
	GS_Out element;
	for (uint i = 0; i < 4; i++)
	{
        
        element.pos = input[0].pos + input[0].scale * quadVertexArray[i];
        float2x2 rot = float2x2(cos(input[0].angle), -sin(input[0].angle), sin(input[0].angle), cos(input[0].angle));
        element.pos.xy = mul(element.pos.xy - input[0].pos.xy, rot) + input[0].pos.xy;
        element.worldPos = mul(cameraWorldMatrix, element.pos);
        element.pos = mul(projectionMatrix, element.pos);
        element.color = float4(input[0].color, -1);
		output.Append(element);
	}
    output.RestartStrip();

}