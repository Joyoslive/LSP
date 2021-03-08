struct Particle
{
    float3 pos;
    float lifeTime;
    float3 vel;
    float scale;
    float3 color;
    float padding;
};

StructuredBuffer<Particle> vertexBuffer : register(t0); //lol hahah den heter vertexBuffer, jättekul

cbuffer Transforms : register(b0)
{
    matrix cameraWorldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VS_IN
{
    uint id : SV_VertexID;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float3 color : COLOR;
    float scale : SCALE;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    Particle p = vertexBuffer.Load(input.id);
    output.pos = mul(viewMatrix, float4(vertexBuffer.Load(input.id).pos, 1));
    output.color = float4(vertexBuffer.Load(input.id).color, -1);
    output.scale = p.scale;
    return output;
}