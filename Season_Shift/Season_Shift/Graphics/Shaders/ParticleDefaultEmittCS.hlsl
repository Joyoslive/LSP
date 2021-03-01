
struct Particle
{
    float3 pos;
    float lifeTime;
    float3 vel;
    float other;
};

AppendStructuredBuffer<Particle> appendBuffer : register(u0);

cbuffer ParticlEmitt : register(b0)
{
    float3 pos;
    float lifeTime;
    float3 randVec;
    float other;
    float3 direction;
    uint count;
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
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint id = DTid.x + DTid.y * size + DTid.z * size * size;
    if (id < count)
    {
        Particle p;
        p.lifeTime = lifeTime;
        p.pos = pos;
        p.vel = reflect(dir[DTid.x], randVec);
        p.other = other;
        
        appendBuffer.Append(p);
    }
}