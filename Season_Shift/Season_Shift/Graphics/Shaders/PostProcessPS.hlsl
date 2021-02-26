// Speedlines
//#define RAD 0.34
//#define THICKNESS 0.00003
//#define BASE_SPEED_FAC 1.4    // change to lower value to see the ease-out easier

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

cbuffer PostProcessVariables : register(b2)
{
	float g_elapsedTime;
	float g_deltaTime;
	int g_clientWidth;
	int g_clientHeight;
	double g_randomNumber;

	// Speedline
	float g_speedlineRAD;
	float g_speedlineThickness;
	float g_speedlineSpeedFactor;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

int MIN = -2147483648;
int MAX = 2147483647;

// ---------- RANDOM NUMBER GENERATOR -------------
// https://www.shadertoy.com/view/tsf3Dn - RNG with seed
int xorshift(int value) {
	// Xorshift*32
	// Based on George Marsaglia's work: http://www.jstatsoft.org/v08/i14/paper
	value ^= value << 13;
	value ^= value >> 17;
	value ^= value << 5;
	return value;
}

int nextInt(inout int seed) {
	seed = xorshift(seed);
	return seed;
}

float nextFloat(int seed) {
	int seed2 = xorshift(seed);
	// FIXME: This should have been a seed mapped from MIN..MAX to 0..1 instead
	return abs(frac(float(seed2) / 3141.592653));
}
// RNG can be replaced with CPU side rand num gen. (every sec)
//

float4 motionBlur(float4 worldPos, float2 uv)
{
	// Detect skybox
	if (worldPos.w == 0.f)
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


	if (length(pb) + length(pa) <= length(ab) + g_speedlineThickness)
	{
		return 1.f;
	}
	else
	{
		return 0.f;
	}
}

float3 drawSpeedLine(float2 uv, int seed, float speedFac, float easeOffset)
{
	float circVal = nextFloat(seed) * 3.14 * 2.;

	float2 endPoint = g_speedlineRAD * float2(cos(circVal), sin(circVal));
	float2 dir = normalize(endPoint - float2(0., 0.));
	float2 startPoint = 3. * dir;

	// ease out animation
	float easeOutMagnitude = fmod(speedFac * easeOffset * abs(float(g_elapsedTime + 10000.)), 1.);

	endPoint += dir * easeOutMagnitude;

	return drawLine(uv, startPoint, endPoint);
}

float4 main(PS_IN input) : SV_TARGET
{
	float3 col = float3(0., 0., 0.);

	// Setup ShaderToy GL style
	float2 uv = input.uv;	// dont use this UV to sample textures from.
	uv.y = 1. - uv.y;
	uv.xy -= float2(0.5, 0.5);

	float3 speedLine = drawSpeedLine(uv, 2, 1., 1.);
	float3 inTex = g_bbTex.Sample(g_sampler, input.uv).xyz;
	
	int t2 = int(g_elapsedTime);
	//float speedFac = g_speedlineSpeedFactor * (nextFloat(g_elapsedTime) * 0.5 + 0.5);
	float speedFac = g_speedlineSpeedFactor;

	int seed = 0;
	float lines = 0.;


	// mod in time for seed may solve the sometimes occurring repeating circular pattern 
	// (not verified)
	for (int i = 0; i < 75; ++i)
	{
		seed = int(fmod((fmod(g_elapsedTime, 7.)) * speedFac, speedFac)) * i * i * i + 5 * i;
		//seed = int(fmod((fmod(g_elapsedTime, 7.)) * speedFac, speedFac)) * i + 3 * i;
		//seed = int(fmod((fmod(g_elapsedTime, 7.)) * speedFac, speedFac)) * i + 3 * g_randomNumber;

		//seed = int(fmod(g_elapsedTime, 7.))  * i * i * i + 5 * i;

		lines += drawSpeedLine(uv, seed, speedFac, nextFloat(seed) * 0.3 + 1.);
	}

	col = float3(lines, lines, lines);

	// Crosshair
	float2 arFixedUv = float2(uv.x * (float(g_clientWidth)/float(g_clientHeight)), uv.y);
	float crosshairRadius = 0.007;
	if (arFixedUv.x * arFixedUv.x + arFixedUv.y * arFixedUv.y <= crosshairRadius * crosshairRadius)
	{
		return float4(1., 0., 0., 1.);
	}

	// motion blur
	float4 worldPos = g_worldPosTex.Sample(g_sampler, input.uv);
	float4 moBlur = motionBlur(worldPos, input.uv);

	return saturate(moBlur + float4(col,1));
}