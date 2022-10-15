#include "TrussTypes.hlsl"


#define thisMember mArray[DTID.x]

[numthreads(MEMBERS_NUM, 1, 1)]
void main(uint3 DTID : SV_GroupThreadID)
{
   thisMember.magnitude_start = (thisMember.magnitude_start + thisMember.magnitude_end) * 0.5f;
   thisMember.magnitude_end =  thisMember.magnitude_start;
    
}