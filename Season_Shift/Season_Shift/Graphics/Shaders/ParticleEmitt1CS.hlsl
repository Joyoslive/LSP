
struct Particle
{
    float3 pos;
    float lifeTime;
    float3 vel;
    float scale;
    float3 color;
    float angle;
};

AppendStructuredBuffer<Particle> appendBuffer : register(u0);

cbuffer ParticlEmitt : register(b0)
{
    float3 pos;
    float lifeTime;
    float3 randVec;
    float scale;
    float3 direction;
    uint count;
    float3 color;
    float angle;
};

cbuffer NumOfParticleBuffer : register(b1)
{
    uint particleCount;
    uint3 padding;
};

float3 random(float seed, float3 seedVec)
{
    float r1 = frac(sin(dot(float2(seed, seedVec.x), float2(12.9898, 78.233))) * 43758.5453123);
    float r2 = frac(sin(dot(float2(r1, seedVec.y), float2(12.9898, 78.233))) * 43758.5453123);
    float r3 = frac(sin(dot(float2(r2, seedVec.z), float2(12.9898, 78.233))) * 43758.5453123);
    return float3(r1, r2, r3);
}

#define size 8

[numthreads(size, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint id = DTid.x + DTid.y * size + DTid.z * size * size;
    uint maxCount, stride;
    appendBuffer.GetDimensions(maxCount, stride);
    if (id < count && id < maxCount - particleCount)
    {
        Particle p;
        p.pos = pos;
        p.pos += 80 * (2 * random((float) DTid.x / (float) count, normalize(randVec)) - float3(1, 1, 1));
        p.vel = float3(0, 0, 0);
        p.scale = scale;
        p.color = color;
        p.lifeTime = lifeTime;
        p.angle = DTid.x;
    
        appendBuffer.Append(p);
    }
}