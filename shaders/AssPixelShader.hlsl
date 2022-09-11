
cbuffer lightSource
{
    float3 lightDir;
    float3 lightColor;
    float3 ambient ;
    float3 diffuseColor;
    float diffuseIntensity ;

};

cbuffer material
{
    float3 color;
};


float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
	
	// diffuse attenuation
	// diffuse intensity
    float3 diffuse =  ( max(0.0f, dot(lightDir, n)) / (length(n) * length(lightDir)));
	// final color
    return float4(saturate(diffuse + ambient) * color, 1.0f);
}