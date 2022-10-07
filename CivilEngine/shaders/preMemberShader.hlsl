
struct Node
{
    float3 pos, externalForce;
    uint num, memberIndecies[5];
};

struct Support
{
    float3 pos, externalForce;
    uint num, memberIndecies[5];
    uint reactionNum;
    float3 reaction[3];
};

struct Member
{
    float3 direction;
    float magnitude_start, magnitude_end;
    uint start, end;
};

RWStructuredBuffer<Node> vArray : register(u0);
RWStructuredBuffer<Member> mArray : register(u1);
RWStructuredBuffer<Support> sArray : register(u2);

#define thisMember mArray[DTID.x]
float3 pos(uint index, uint num)
{
    if (index > num)
    {
        return sArray[index -num].pos;
    }
    return vArray[index].pos;
}
/*
// need attomic functions

void attachMember(uint memberIndex, uint vertexIndex, uint num)
{
    if (vertexIndex > num)
    {
        sArray[vertexIndex - num].memberIndecies[sArray[vertexIndex - num].num] = memberIndex;
        sArray[vertexIndex - num].num++;

    }
    else
    {
        vArray[vertexIndex].memberIndecies[vArray[num].num] = memberIndex;
        vArray[vertexIndex].num++;
    }
}
*/

[numthreads(7, 1, 1)]
void main(uint3 DTID : SV_GroupThreadID)
{
    uint num = 3;
    
    thisMember.direction = normalize(pos(thisMember.end, num) - pos(thisMember.start, num));
   // attachMember(DTID.x, thisMember.start, num);
    //attachMember(DTID.x, thisMember.end, num);
    
}
