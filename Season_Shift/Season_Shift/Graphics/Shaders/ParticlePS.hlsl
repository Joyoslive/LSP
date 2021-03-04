
struct PS_Out
{
    float4 positionWS : SV_TARGET0;
    float4 positionNormal : SV_TARGET1;
    float4 uv : SV_TARGET2;
    float4 diffuseColor : SV_TARGET3;
};

struct GS_Out
{
    float4 pos : SV_Position;
    float4 worldPos : WORLDPOS;
    float4 color : COLOR;
};

PS_Out main(GS_Out input)
{
    PS_Out output = (PS_Out) 0;
    output.positionWS = input.worldPos;
    output.diffuseColor = input.color;
    output.positionNormal = float4(0, 0, -1, 0);
    output.uv = float4(0, 0, 0, 1);
    return output;
}