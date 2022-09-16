
cbuffer tmat {
	matrix transform;
    matrix transform1;

};

float4 main( float3 pos : POSITION) : SV_Position
{
	 
	 return	 mul(float4(pos, 1.0f), transform1);
	
}