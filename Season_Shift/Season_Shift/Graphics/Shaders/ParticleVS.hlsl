struct Particle
{
    float3 pos;
    float lifeTime;
    float3 vel;
    float other;
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
};

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    output.pos = mul(viewMatrix, float4(vertexBuffer.Load(input.id).pos, 1));
    return output;
}