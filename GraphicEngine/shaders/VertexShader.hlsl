cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};


float4 main( float3 pos : POSITION ) : SV_POSITION
{
    return mul(float4(pos, 1.0f), modelViewProj);
    //return float4(pos * 0.25, 1.0);
}