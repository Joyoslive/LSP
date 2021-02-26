struct PS_OUT
{
    float4 positionWS : SV_TARGET0;
    float4 positionNormal : SV_TARGET1;
    float4 uv : SV_TARGET2;
    float4 diffuseColor : SV_TARGET3;
};

struct GS_OUT
{
    float4 pos : SV_POSITION;
    float3 col : COLOR;
};

PS_OUT main(GS_OUT input)
{
    PS_OUT output = (PS_OUT)0;

    output.positionWS = float4(0., 0., 0., 0.);
    output.positionNormal = float4(0., 0., 0., 1.);
    output.uv = float4(0., 0., 0., 1.);
    //output.diffuseColor = float4(1.0f, 1.0f, 1.0f, -1.f);   // dont take lighting into account (w: -1)
    output.diffuseColor = float4(input.col, -1.f); // dont take lighting into account (w: -1)

    return output;
}