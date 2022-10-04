sampler smplr;

Texture2D DiffuseMap;
Texture2D ampientMap;
Texture2D Specular;


cbuffer lightSource
{
    float3 lightDir;
    float3 lightColor;
    float3 ambientColor;
    float diffuseIntensity ;
    float ambientIntensity;

};

//Buffer<float> p;
cbuffer MaterialData
{
    float4 DiffuseColor, AmpientColor, SpecularColor;
};

struct VSINPUT
{
    float3 normal : NORMAL;
    float2 tex2d : TEXCOORD;
};


float4 main(VSINPUT vin) : SV_Target
{

    float4 ampientTexData = normalize(float4(ambientColor, 0.0f)) * ampientMap.Sample(smplr, vin.tex2d) * ambientIntensity;
//    float4 ampientTexData = normalize(float4(ambientColor, 0.0f)) * AmpientColor * ambientIntensity;

    float diffuse = diffuseIntensity * dot(vin.normal, lightDir) / (length(vin.normal) * length(lightDir));
    float4 diffuseTexData = diffuse * normalize(float4(lightColor, 0.0f)) * DiffuseMap.Sample(smplr, vin.tex2d) / length(lightColor);
    
    return  ampientTexData + diffuseTexData;
   
}