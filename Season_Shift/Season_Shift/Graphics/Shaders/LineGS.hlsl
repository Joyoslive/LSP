struct GSOutput
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float3 col : COLOR;
};

cbuffer CamInfo : register(b0)
{
	matrix g_camVM;	// to generate with
	matrix g_camPM;
	float3 g_color;
	float g_thicknessStart;
	float3 g_startOffset;
	float g_thicknessEnd;
	matrix g_camVMForRender;	// to render with
}

[maxvertexcount(24)]
void main(
	line float4 input[2] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	// World space directions of cam
	float4 forwardDir = float4(g_camVM._31, g_camVM._32, g_camVM._33, 0.);
	float4 rightDir = float4(g_camVM._11, g_camVM._12, g_camVM._13, 0.);
	float4 upDir = float4(g_camVM._21, g_camVM._22, g_camVM._23, 0.);
	
	// Quad offset
	float4 startOffset = float4(0., 2., 3., 0.);

	startOffset = float4(g_startOffset, 1.);

	/*
	What we want instead:

	input[0] is startPos in WS
	
	tlStartWorldSpace = input[0] + (forwardDir x offsetZ) + (rightDir x offsetX) + (upDir x offsetY)	
	--> The dirs (from view cam) used here should be the from the view cam that the geometry is CREATED FOR!
	
	tlStartViewSpace = mul(g_camVM, tlStartWorldSpace);		--> The cam used here should be the one to RENDER To

	*/

	// start quad
	float4 tlStartWS = input[0] + (-g_thicknessStart * rightDir) + (g_thicknessStart * upDir);
	float4 trStartWS = input[0] + (g_thicknessStart * rightDir) + (g_thicknessStart * upDir);
	float4 blStartWS = input[0] + (-g_thicknessStart * rightDir) + (-g_thicknessStart * upDir);
	float4 brStartWS = input[0] + (g_thicknessStart * rightDir) + (-g_thicknessStart * upDir);

	tlStartWS += rightDir * startOffset.x + upDir * startOffset.y + forwardDir * startOffset.z;
	trStartWS += rightDir * startOffset.x + upDir * startOffset.y + forwardDir * startOffset.z;
	blStartWS += rightDir * startOffset.x + upDir * startOffset.y + forwardDir * startOffset.z;
	brStartWS += rightDir * startOffset.x + upDir * startOffset.y + forwardDir * startOffset.z;

	float4 tlStart = mul(g_camVMForRender, tlStartWS);
	float4 trStart = mul(g_camVMForRender, trStartWS);
	float4 blStart = mul(g_camVMForRender, blStartWS);
	float4 brStart = mul(g_camVMForRender, brStartWS);
	tlStart = mul(g_camPM, tlStart);
	trStart = mul(g_camPM, trStart);
	blStart = mul(g_camPM, blStart);
	brStart = mul(g_camPM, brStart);

	float3 tlStartNormal = normalize(-forwardDir - rightDir + upDir).xyz;
	float3 trStartNormal = normalize(-forwardDir + rightDir + upDir).xyz;
	float3 blStartNormal = normalize(-forwardDir - rightDir - upDir).xyz;
	float3 brStartNormal = normalize(-forwardDir + rightDir - upDir).xyz;

	GSOutput element;
	element.col = g_color;

	element.normal = tlStartNormal;
	element.pos = tlStart;
	output.Append(element);

	element.normal = trStartNormal;
	element.pos = trStart;
	output.Append(element);

	element.normal = blStartNormal;
	element.pos = blStart;
	output.Append(element);

	element.normal = brStartNormal;
	element.pos = brStart;
	output.Append(element);

	output.RestartStrip();

	// end quad
	float4 tlEndWS = input[1] + (-g_thicknessEnd * rightDir) + (g_thicknessEnd * upDir);
	float4 trEndWS = input[1] + (g_thicknessEnd * rightDir) + (g_thicknessEnd * upDir);
	float4 blEndWS = input[1] + (-g_thicknessEnd * rightDir) + (-g_thicknessEnd * upDir);
	float4 brEndWS = input[1] + (g_thicknessEnd * rightDir) + (-g_thicknessEnd * upDir);

	float4 tlEnd = mul(g_camVMForRender, tlEndWS);
	float4 trEnd = mul(g_camVMForRender, trEndWS);
	float4 blEnd = mul(g_camVMForRender, blEndWS);
	float4 brEnd = mul(g_camVMForRender, brEndWS);
	tlEnd = mul(g_camPM, tlEnd);
	trEnd = mul(g_camPM, trEnd);
	blEnd = mul(g_camPM, blEnd);
	brEnd = mul(g_camPM, brEnd);

	float3 tlEndNormal = normalize(forwardDir - rightDir + upDir).xyz;
	float3 trEndNormal = normalize(forwardDir + rightDir + upDir).xyz;
	float3 blEndNormal = normalize(forwardDir - rightDir - upDir).xyz;
	float3 brEndNormal = normalize(forwardDir + rightDir - upDir).xyz;

	element.normal = tlEndNormal;
	element.pos = tlEnd;
	output.Append(element);

	element.normal = trEndNormal;
	element.pos = trEnd;
	output.Append(element);

	element.normal = blEndNormal;
	element.pos = blEnd;
	output.Append(element);

	element.normal = brEndNormal;
	element.pos = brEnd;
	output.Append(element);

	output.RestartStrip();

	///*
	//Below: Handle the sides that make up the rectangle
	//*/

	// right
	element.normal = trStartNormal;
	element.pos = trStart;
	output.Append(element);

	element.normal = trEndNormal;
	element.pos = trEnd;
	output.Append(element);

	element.normal = brStartNormal;
	element.pos = brStart;
	output.Append(element);

	element.normal = brEndNormal;
	element.pos = brEnd;
	output.Append(element);

	output.RestartStrip();


	// top

	element.normal = tlStartNormal;
	element.pos = tlStart;
	output.Append(element);

	element.normal = tlEndNormal;
	element.pos = tlEnd;
	output.Append(element);

	element.normal = trStartNormal;
	element.pos = trStart;
	output.Append(element);

	element.normal = trEndNormal;
	element.pos = trEnd;
	output.Append(element);

	output.RestartStrip();


	// bottom

	element.normal = blEndNormal;
	element.pos = blEnd;
	output.Append(element);

	element.normal = blStartNormal;
	element.pos = blStart;
	output.Append(element);

	element.normal = brEndNormal;
	element.pos = brEnd;
	output.Append(element);

	element.normal = brStartNormal;
	element.pos = brStart;
	output.Append(element);

	output.RestartStrip();

	// left
	element.normal = tlEndNormal;
	element.pos = tlEnd;
	output.Append(element);

	element.normal = tlStartNormal;
	element.pos = tlStart;
	output.Append(element);

	element.normal = blEndNormal;
	element.pos = blEnd;
	output.Append(element);

	element.normal = blStartNormal;
	element.pos = blStart;
	output.Append(element);



}