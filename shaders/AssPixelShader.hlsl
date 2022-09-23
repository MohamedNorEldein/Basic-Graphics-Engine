
cbuffer lightSource
{
    float3 lightDir;
    float3 lightColor;
    float3 ambient ;
    float diffuseIntensity ;
    float ambientIntensity;

};

cbuffer material
{
    float3 color ;
};


float4 main( float3 normal : Normal) : SV_Target
{
	
    float diffuse = diffuseIntensity * dot(normal, lightDir) / (length(normal) * length(lightDir));
    
    return float4(saturate(ambient * ambientIntensity + diffuse * color * lightColor), 1.0f);
}