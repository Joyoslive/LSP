Texture2D g_posTex : register(t0);
Texture2D g_norTex : register(t1);
Texture2D g_uvTex : register(t2);
Texture2D g_difTex : register(t3);

Texture1D g_discTex : register(t10);
Texture2D g_smNear : register(t6);
Texture2D g_smMid : register(t7);
Texture2D g_smFar : register(t8);

SamplerState g_sampler : register(s0);
SamplerState g_smBorderSampler : register(s1);

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer dirLight : register(b0)
{
	float3 g_dlDirection;
	float g_dlAmbIntensity;
	float4 g_dlColor;
};

cbuffer camBuf : register(b1)
{
	float3 g_camPos;
};

cbuffer cascadeBuffer : register(b7)
{
	float g_nearCascadeEnd;
	float g_midCascadeEnd;
	float g_farCascadeEnd;
	float padding;

	matrix g_mainViewMatrix;

	matrix g_lightViewMatrix;
	matrix g_lightNearProjection;
	matrix g_lightMidProjection;
	matrix g_lightFarProjection;
}

float4 calcDirLight(float3 normal, float3 worldPos, float4 texColor)
{
	float3 lDir = normalize(-g_dlDirection);

	float4 amb = saturate(g_dlColor * g_dlAmbIntensity);

	// Diffuse intensity sampled from discretization map
	float incidentIntensity = saturate(dot(normal, lDir));
	incidentIntensity = g_discTex.Sample(g_sampler, incidentIntensity);
	float4 dif = saturate(g_dlColor * incidentIntensity);

	float3 camDir = normalize(g_camPos - worldPos);
	float3 halfWay = normalize(camDir + lDir);

	// Specular intensity sampled from discretization map
	float specInt = pow(max(dot(normal, halfWay), 0), 32);
	specInt = g_discTex.Sample(g_sampler, specInt);
	float4 spec = saturate(g_dlColor * specInt);	

	return saturate((amb + dif + spec) * texColor);
}

float4 calcShadow(float3 worldPos, float4 inputColor, float3 normal, float2 uv)
{
	float4 camVS = mul(g_mainViewMatrix, float4(worldPos, 1.f));

	//float4 lcs = mul(g_lightMidProjection, mul(g_lightViewMatrix, float4(worldPos, 1.)));
	//float2 smUv = float2(0.5f * lcs.x + 0.5f, -0.5f * lcs.y + 0.5f);
	//float depth = lcs.z;

	float4 lcs = float4(0., 0., 0., 0.);
	float2 smUv = float2(0., 0.);
	float depth = 1.;

	float2 texelSize = float2(0., 0.);
	float width;
	float height;

	float cascadeEnds[3] = { g_nearCascadeEnd, g_midCascadeEnd, g_farCascadeEnd };

	float dotLightNor = abs(dot(g_dlDirection, normal));	// [0, 1]

	float shadowMapDepth = 0.f;
	float bias = 0.f;
	float4 tmpCol = float4(0., 0., 0., 0.);

	float shadowFac = 0.07f;
	float shadow = 0.f;
	for (int i = 0; i < 3; i++)
	{
		if (camVS.z <= cascadeEnds[i])
		{
			if (i == 0)
			{
				lcs = mul(g_lightNearProjection, mul(g_lightViewMatrix, float4(worldPos, 1.)));
				smUv = float2(0.5f * lcs.x + 0.5f, -0.5f * lcs.y + 0.5f);
				depth = lcs.z;

				//bias = 0.0008f;
				bias = max(0.0015 * (1.0 - dotLightNor) , 0.0009f);

				//return float4(dotLightNor, dotLightNor, dotLightNor, 1.f);

				shadowMapDepth = g_smNear.Sample(g_smBorderSampler, smUv).r;
				tmpCol = float4(1.f, 0.f, 0.f, 1.f);

				//Get dimension of the texture
				g_smNear.GetDimensions(width, height);
				texelSize = float2(1.f, 1.f) / float2(width, height);
				//Loop through the 9 neighbours and check the depth and if the depth is bigger than we add shadowFac
				for (int x = -1; x <= 1; ++x)
				{
					for (int y = -1; y <= 1; ++y)
					{
						float pcfDepth = g_smNear.Sample(g_smBorderSampler, smUv + float2(x, y) * texelSize).r;
						shadow += depth - bias > pcfDepth ? shadowFac : 0.0f;
					}
				}
				break;
			}
			else if (i == 1)
			{
				if (camVS.z > cascadeEnds[i - 1])
				{
					lcs = mul(g_lightMidProjection, mul(g_lightViewMatrix, float4(worldPos, 1.)));
					smUv = float2(0.5f * lcs.x + 0.5f, -0.5f * lcs.y + 0.5f);
					depth = lcs.z;

					bias = 0.001f;//0.0005f;
					shadowMapDepth = g_smMid.Sample(g_smBorderSampler, smUv).r;
					tmpCol = float4(0.f, 1.f, 0.f, 1.f);

					//Get dimension of the texture
					g_smMid.GetDimensions(width, height);
					texelSize = float2(1.f, 1.f) / float2(width, height);
					//Loop through the 9 neighbours and check the depth and if the depth is bigger than we add shadowFac
					for (int x = -1; x <= 1; ++x)
					{
						for (int y = -1; y <= 1; ++y)
						{
							float pcfDepth = g_smMid.Sample(g_smBorderSampler, smUv + float2(x, y) * texelSize).r;
							shadow += depth - bias > pcfDepth ? shadowFac : 0.0f;
						}
					}
					break;
				}
			}
			else
			{
				if (camVS.z > cascadeEnds[i - 1])
				{
					lcs = mul(g_lightFarProjection, mul(g_lightViewMatrix, float4(worldPos, 1.)));
					smUv = float2(0.5f * lcs.x + 0.5f, -0.5f * lcs.y + 0.5f);
					depth = lcs.z;

					bias = 0.01f;//0.001f;
					shadowMapDepth = g_smFar.Sample(g_smBorderSampler, smUv).r;
					tmpCol = float4(0.f, 0.f, 1.f, 1.f);

					//Get dimension of the texture
					g_smFar.GetDimensions(width, height);
					texelSize = float2(1.f, 1.f) / float2(width, height);
					//Loop through the 9 neighbours and check the depth and if the depth is bigger than we add shadowFac
					for (int x = -1; x <= 1; ++x)
					{
						for (int y = -1; y <= 1; ++y)
						{
							float pcfDepth = g_smFar.Sample(g_smBorderSampler, smUv + float2(x, y) * texelSize).r;
							shadow += depth - bias > pcfDepth ? shadowFac : 0.0f;
						}
					}
					break;
				}
			}
		}
	}

	shadow /= 9.0f;
	shadow = shadowFac*2.0f - shadow;

	// check edge of platform (artifact or bias?)
	float4 col = inputColor;

	if (shadow != shadowFac * 2.0f)
		col = g_difTex.Sample(g_sampler, uv) * shadow;
	else
		col = inputColor;

	//Old depthCheck
	/*if (depth > shadowMapDepth + bias)	// shadowed
		col = g_difTex.Sample(g_sampler, uv) * shadowFac;
	else
		col = inputColor;*/
		
	//col = inputColor * shadowed;
	col += tmpCol * 0.08;
	return col;
}

float4 main(PS_IN input) : SV_TARGET
{
	float4 finalColor = g_difTex.Sample(g_sampler, input.uv);
	float3 normal = normalize(g_norTex.Sample(g_sampler, input.uv).xyz);
	float3 worldPos = g_posTex.Sample(g_sampler, input.uv).xyz;

	//return float4(normal, 1.f);

	if (!(finalColor.w > -1.1 && finalColor.w < -0.9))
	{
		finalColor = calcDirLight(normal, worldPos, finalColor);
		finalColor = calcShadow(worldPos, finalColor, normal, input.uv);
	}


	
	finalColor = pow(finalColor, float4(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f, 1.f));
	return finalColor;
}