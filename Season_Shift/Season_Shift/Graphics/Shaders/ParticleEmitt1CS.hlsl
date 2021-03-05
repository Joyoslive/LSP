
struct Particle
{
    float3 pos;
    float lifeTime;
    float3 vel;
    float scale;
    float3 color;
    float padding;
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
    float padding1;
};

cbuffer NumOfParticleBuffer : register(b1)
{
    uint particleCount;
    uint3 padding;
};

static const float3 dir[8] =
{
    normalize(float3(1.0f, 1.0f, 1.0f)),
    normalize(float3(1.0f, 1.0f, -1.0f)),
    normalize(float3(1.0f, -1.0f, 1.0f)),
    normalize(float3(1.0f, -1.0f, -1.0f)),
    normalize(float3(-1.0f, 1.0f, 1.0f)),
    normalize(float3(-1.0f, 1.0f, -1.0f)),
    normalize(float3(-1.0f, -1.0f, 1.0f)),
    normalize(float3(-1.0f, -1.0f, -1.0f))
};


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
        p.lifeTime = lifeTime;
        p.pos = pos;
        p.vel = 2 * normalize(reflect(dir[DTid.x], randVec));
        p.pos += 10 * (randVec + p.vel);
        p.scale = scale;
        p.color = color;
        p.padding = 0;
        
        appendBuffer.Append(p);
    }
}