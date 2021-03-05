
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
ConsumeStructuredBuffer<Particle> consumeBuffer : register(u1);

cbuffer SimulationInfo : register(b0)
{
    float dt;
    float maxLifeTime;
    float2 padding1;
}

cbuffer NumOfParticleBuffer : register(b1)
{
    uint particleCount;
    uint3 padding;
};

#define size 1024
[numthreads(size, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint id = DTid.x + DTid.y * size + DTid.z * size * size;
    if (id < particleCount)
    {
        Particle p = consumeBuffer.Consume();
        p.vel.y -= dt;
        p.pos += p.vel * dt;
        p.lifeTime += dt;
        if(p.lifeTime < maxLifeTime)
        {
            appendBuffer.Append(p);
        }
    }
}