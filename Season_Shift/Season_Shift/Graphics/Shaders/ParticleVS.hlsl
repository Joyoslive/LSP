struct VS_IN
{
    float4 pos : POSITION;
};

struct VS_OUT
{
    float4 pos : SV_Position;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    output.pos = input.pos;
    return output;
}