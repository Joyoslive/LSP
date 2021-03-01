struct GSOutput
{
	float4 pos : SV_POSITION;
	float3 col : COLOR;
};

cbuffer CamInfo : register(b0)
{
	matrix g_camVM;
	matrix g_camPM;
	float3 g_color;
	float g_thicknessStart;
	float3 g_startOffset;
	float g_thicknessEnd;

}


[maxvertexcount(24)]		// temp value
void main(
	line float4 input[2] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	// move to right hand
	//float thickness = 0.1;
	float4 startOffset = float4(1., 0.4, 0., 0.);

	startOffset = float4(g_startOffset, 1.);

	// start quad
	float4 tlStart = mul(g_camVM, input[0]) + float4(-g_thicknessStart, g_thicknessStart, 0, 0) + startOffset;
	float4 trStart = mul(g_camVM, input[0]) + float4(g_thicknessStart, g_thicknessStart, 0, 0) + startOffset;
	float4 blStart = mul(g_camVM, input[0]) + float4(-g_thicknessStart, -g_thicknessStart, 0, 0) + startOffset;
	float4 brStart = mul(g_camVM, input[0]) + float4(g_thicknessStart, -g_thicknessStart, 0, 0) + startOffset;
	tlStart = mul(g_camPM, tlStart);
	trStart = mul(g_camPM, trStart);
	blStart = mul(g_camPM, blStart);
	brStart = mul(g_camPM, brStart);

	GSOutput element;
	//element.col = float3(1., 0., 0.);
	element.col = g_color;

	element.pos = tlStart;
	output.Append(element);

	element.pos = trStart;
	output.Append(element);

	element.pos = blStart;
	output.Append(element);

	element.pos = brStart;
	output.Append(element);

	output.RestartStrip();

	// end quad
	float4 tlEnd = mul(g_camVM, input[1]) + float4(-g_thicknessEnd, g_thicknessEnd, 0, 0);
	float4 trEnd = mul(g_camVM, input[1]) + float4(g_thicknessEnd, g_thicknessEnd, 0, 0);
	float4 blEnd = mul(g_camVM, input[1]) + float4(-g_thicknessEnd, -g_thicknessEnd, 0, 0);
	float4 brEnd = mul(g_camVM, input[1]) + float4(g_thicknessEnd, -g_thicknessEnd, 0, 0);
	tlEnd = mul(g_camPM, tlEnd);
	trEnd = mul(g_camPM, trEnd);
	blEnd = mul(g_camPM, blEnd);
	brEnd = mul(g_camPM, brEnd);

	//element.col = float3(1., 0., 0.);
	element.col = g_color;

	element.pos = tlEnd;
	output.Append(element);

	element.pos = trEnd;
	output.Append(element);

	element.pos = blEnd;
	output.Append(element);

	element.pos = brEnd;
	output.Append(element);

	output.RestartStrip();

	/*
	Below: Handle the sides that make up the rectangle
	*/

	// right
	//element.col = float3(0., 0., 1.);
	element.col = g_color;

	element.pos = trStart;
	output.Append(element);

	element.pos = trEnd;
	output.Append(element);

	element.pos = brStart;
	output.Append(element);

	element.pos = brEnd;
	output.Append(element);

	output.RestartStrip();


	// top
	//element.col = float3(0., 1., 1.);
	element.col = g_color;

	element.pos = tlStart;
	output.Append(element);

	element.pos = tlEnd;
	output.Append(element);

	element.pos = trStart;
	output.Append(element);

	element.pos = trEnd;
	output.Append(element);

	output.RestartStrip();


	// bottom
	//element.col = float3(0., 1., 1.);
	element.col = g_color;

	element.pos = blEnd;
	output.Append(element);

	element.pos = blStart;
	output.Append(element);

	element.pos = brEnd;
	output.Append(element);

	element.pos = brStart;
	output.Append(element);

	output.RestartStrip();

	// left
	//element.col = float3(1., 0., 1.);
	element.col = g_color;

	element.pos = tlEnd;
	output.Append(element);

	element.pos = tlStart;
	output.Append(element);

	element.pos = blEnd;
	output.Append(element);

	element.pos = blStart;
	output.Append(element);






	//for (uint i = 0; i < 2; i++)
	//{
	//	//float4 viewSpacePos = mul(g_camVM, input[i]);
	//	float4 viewSpacePos = input[i];

	//	// Expand point to quad
	//	float4 tl = viewSpacePos + float4(-1, 1, 0, 0);
	//	float4 tr = viewSpacePos + float4(1, 1, 0, 0);
	//	float4 bl = viewSpacePos + float4(-1, -1, 0, 0);
	//	float4 br = viewSpacePos + float4(1, -1, 0, 0);

	//	tl = mul(g_camPM, tl);
	//	tr = mul(g_camPM, tr);
	//	bl = mul(g_camPM, bl);
	//	br = mul(g_camPM, br);

	//	GSOutput element;

	//	element.pos = tl;
	//	output.Append(element);

	//	element.pos = tr;
	//	output.Append(element);

	//	element.pos = bl;
	//	output.Append(element);

	//	element.pos = br;
	//	output.Append(element);

	//	output.RestartStrip();
	//}
}