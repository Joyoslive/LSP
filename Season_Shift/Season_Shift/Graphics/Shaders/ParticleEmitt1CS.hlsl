
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


// ---------- RANDOM NUMBER GENERATOR -------------
// https://www.shadertoy.com/view/tsf3Dn - RNG with seed
int xorshift(int value)
{
	// Xorshift*32
	// Based on George Marsaglia's work: http://www.jstatsoft.org/v08/i14/paper
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;
    return value;
}

float randFloat(int seed)
{
    return float(xorshift(seed)) * (1.0 / 4294967296.0);
}

#define size 8

[numthreads(size, 1, 1)]

    void main
    (
    uint3 DTid : SV_DispatchThreadID)
{
        uint id = DTid.x + DTid.y * size + DTid.z * size * size;
        uint maxCount, stride;
        appendBuffer.GetDimensions(maxCount, stride);
        if (id < count && id < maxCount - particleCount && id < size)
        {
            Particle p;
        
        
            p.pos = pos;
            p.pos += 40 * (reflect(dir[DTid.x], randVec));
            //p.pos += 4000 * float3(randFloat(randVec.x + DTid.x), randFloat(randVec.y + DTid.x), randFloat(randVec.z + DTid.x));
        
            p.vel = float3(0, 0, 0);
        
            p.scale = scale;
            p.color = color;
            p.lifeTime = lifeTime;
            p.angle = DTid.x;
        
            appendBuffer.Append(p);
        }
    }