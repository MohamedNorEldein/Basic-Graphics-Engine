cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

struct VSOut
{
    float3 normal : Normal;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION, float3 normal : NORMAL)
{
    VSOut vso;
    vso.normal = mul(float4(normal, 0.0), model);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    
    return vso;
}