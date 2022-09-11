
cbuffer cb {
	float3 faceColor[6];
    float3 faceNormal[12];
};



static const float3 light = { -1, -1, -1 };

float4 main(uint id : SV_PrimitiveID) : SV_Target
{
    
    float coff = abs(dot(light, faceNormal[id]) / (length(light) * length(faceNormal[id])));
    return float4(coff * faceColor[0], 1.0);

}