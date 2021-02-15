struct VS_OUT
{
    float4 pos : SV_Position;
    float localPos : POSITION;
};

TextureCube g_texCube : register(t5);
SamplerState g_skySampler: register(s5);

float4 main(VS_OUT input) : SV_TARGET
{
    return g_texCube.Sample(g_skySampler, input.localPos);
}