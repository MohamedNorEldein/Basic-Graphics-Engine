
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

float4 main(float4 normal : NORMAL, float4 tex2 : TEXCOORD) : SV_Target
{
	
    float diffuse = diffuseIntensity * dot(normal.rgb, lightDir) / (length(normal) * length(lightDir));
    
    //return float4(saturate(normalize(ambient) * ambientIntensity + diffuse * lightColor) * color, 1.0f);
    return text.Sample(smplr, tex2);
}