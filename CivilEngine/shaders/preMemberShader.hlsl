#include "TrussTypes.hlsl"


#define thisMember mArray[DTID.x]
float3 pos(uint index)
{  
    return vArray[index].pos;
}

// need attomic functions

/*

void attachMember(uint memberIndex, uint vertexIndex)
{
    
    while (vArray[vertexIndex].ocubied==true)
    {
    }
    vArray[vertexIndex].ocubied = true;
    vArray[vertexIndex].memberIndecies[vArray[vertexIndex].num] = memberIndex;
    vArray[vertexIndex].num++;
    vArray[vertexIndex].ocubied = false;
}
*/

[numthreads(MEMBERS_NUM, 1, 1)]
void main(uint3 DTID : SV_GroupThreadID)
{
    
    thisMember.direction = normalize(pos(thisMember.end) - pos(thisMember.start));
  //  attachMember(DTID.x, thisMember.start);
   // attachMember(DTID.x, thisMember.end);
    
}
