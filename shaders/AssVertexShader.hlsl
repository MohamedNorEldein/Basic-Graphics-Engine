cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

struct VSINPUT
{
    float3 pos : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
};


struct VSOut
{
    float3 normal : NORMAL;
    float2 tex2d : TEXCOORD;
    float4 pos : SV_Position;
    
};



VSOut main(VSINPUT vin)
{
    VSOut vso;
    vso.normal = mul(float4(vin.Normal, 0.0), model);
    vso.pos = mul(float4(vin.pos, 1.0f), modelViewProj);
    vso.tex2d = vin.Tex;
    return vso;
}