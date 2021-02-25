struct GSOutput
{
	float4 pos : SV_POSITION;
};

cbuffer CamInfo : register(b0)
{
	matrix g_camVM;
	matrix g_camPM;
}


[maxvertexcount(8)]		// temp value
void main(
	line float4 input[2] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 2; i++)
	{
		float4 viewSpacePos = mul(g_camVM, input[i]);

		// Expand point to quad
		float4 tl = viewSpacePos + float4(-1, 1, 0, 0);
		float4 tr = viewSpacePos + float4(1, 1, 0, 0);
		float4 bl = viewSpacePos + float4(-1, -1, 0, 0);
		float4 br = viewSpacePos + float4(1, -1, 0, 0);

		//tl = mul(g_camPM, tl);
		//tr = mul(g_camPM, tr);
		//bl = mul(g_camPM, bl);
		//br = mul(g_camPM, br);

		GSOutput element;

		element.pos = tl;
		output.Append(element);

		element.pos = tr;
		output.Append(element);

		element.pos = bl;
		output.Append(element);

		element.pos = br;
		output.Append(element);

		output.RestartStrip();
	}
}