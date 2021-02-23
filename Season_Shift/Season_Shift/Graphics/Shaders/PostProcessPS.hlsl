// Speedlines
#define RAD 0.4
#define THICKNESS 0.00003
#define BASE_SPEED_FAC 1.3    // change to lower value to see the ease-out easier

SamplerState g_sampler : register(s0);
Texture2D g_bbTex : register(t0);

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

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


	return float4(col, 1.);
}