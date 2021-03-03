
cbuffer Transforms : register(b0)
{
    matrix cameraWorldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VS_OUT
{
    float4 pos : SV_Position;
};


struct GS_Out
{
	float4 pos : SV_Position;
    float4 worldPos : WORLDPOS;
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
        element.pos = input[0].pos + 0.2f * quadVertexArray[i];
        element.worldPos = mul(cameraWorldMatrix, element.pos);
        element.pos = mul(projectionMatrix, element.pos);
		output.Append(element);
	}
    output.RestartStrip();

}