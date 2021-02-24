// Speedlines
#define RAD 0.4
#define THICKNESS 0.00003
#define BASE_SPEED_FAC 1.3    // change to lower value to see the ease-out easier

#define MOBLUR_SAMPLES 3

SamplerState g_sampler : register(s0);
Texture2D g_bbTex : register(t0);
Texture2D g_worldPosTex : register(t1);

cbuffer MatrixBuffer : register(b0)
{
	matrix g_wMatrix;
	matrix g_vMatrix;
	matrix g_pMatrix;
};

cbuffer PrevMatrices : register(b1)
{
	matrix g_prevView;
	matrix g_prevProj;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 motionBlur(float4 worldPos, float2 uv)
{
	// Detect skybox
	if (worldPos.w == 0)
	{
		return g_bbTex.Sample(g_sampler, uv);
	}
	float4 currentPos = mul(g_pMatrix, mul(g_vMatrix, worldPos));
	currentPos /= currentPos.w;

	float4 prevPos = mul(g_prevProj, mul(g_prevView, worldPos));
	prevPos /= prevPos.w;

	float4 velocity = (currentPos - prevPos) / 2.0;

	float4 color = g_bbTex.Sample(g_sampler, uv);
	uv += velocity;
	for (int i = 1; i < MOBLUR_SAMPLES; ++i, uv += velocity)
	{
		color += g_bbTex.Sample(g_sampler, uv);
	}

	return color / (float)MOBLUR_SAMPLES;
}

float drawLine(float2 P, float2 A, float2 B)
{
	float2 ab = B - A;
	float2 pb = B - P;
	float2 pa = A - P;


	if (length(pb) + length(pa) <= length(ab) + THICKNESS)
	{
		return 1.f;
	}
	else
	{
		return 0.f;
	}
}

float3 drawSpeedLine(float2 p)
{
	//float l = 0;
	//l = drawLine(p, float2(-0.5, -0.5), float2(0., 0.));
	//return float3(l, l, l);

	// The key here is we have some random number every x seconds.
	//float circVal = nextFloat(seed) * 3.14 * 2.;
	float circVal = -0.3;	// temp

	float2 endPoint = RAD * float2(cos(circVal), sin(circVal));
	float2 dir = normalize(endPoint - float2(0., 0.));
	float2 startPoint = 3. * dir;

	// ease out animation
	float tempTime = 0.;
	float tempEaseOffset = 1.;
	float tempSpeedFac = 1.;

	float easeOutMagnitude = fmod(tempSpeedFac * tempEaseOffset * abs(float(tempTime + 10000.)), 1.);

	// This can work too!
	//float easeOutMagnitude = mod(speedFac * easeOffset * abs(float(iTime*iTime/1000. + 10000.)), 1.);

	endPoint += dir * easeOutMagnitude;

	return drawLine(p, startPoint, endPoint);


}



float4 main(PS_IN input) : SV_TARGET
{
	float3 col = float3(0., 0., 0.);

	// Setup ShaderToy GL style
	float2 uv = input.uv;	// dont use this UV to sample textures from.
	uv.y = 1. - uv.y;
	uv.xy -= float2(0.5, 0.5);

	float3 speedLine = drawSpeedLine(uv);
	float3 inTex = g_bbTex.Sample(g_sampler, input.uv).xyz;

	

	//col = max(speedLine, inTex);

	col = max(float3(uv, 0.), speedLine);

	float4 worldPos = g_worldPosTex.Sample(g_sampler, input.uv);
	float4 moBlur = motionBlur(worldPos, input.uv);

	return saturate(moBlur + float4(col,1));
}