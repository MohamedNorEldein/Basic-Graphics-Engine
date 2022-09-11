
cbuffer cb {
	float4 faceColor[2];
	uint TrianglesNum;

};

float4 main(uint id : SV_PrimitiveID) : SV_Target
{
	if (id < TrianglesNum) {
		return faceColor[0];
	}
return faceColor[1];

}