
Texture2D text;

sampler smplr;

cbuffer lightSource
{
    float3 lightDir;
    float3 lightColor;
    float3 ambient;
    float diffuseIntensity;
    float ambientIntensity;

};


struct VSINPUT
{
    float3 normal : NORMAL;
    float2 tex2d : TEXCOORD;
};


float4 main(VSINPUT vin) : SV_Target
{
	
    float diffuse = diffuseIntensity * dot(vin.normal, lightDir) / (length(vin.normal) * length(lightDir));
    
    return float4(normalize(ambient) * ambientIntensity + diffuse * lightColor, 1.0) * text.Sample(smplr, vin.tex2d);
}