
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
        
    }
}