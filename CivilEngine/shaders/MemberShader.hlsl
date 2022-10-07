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

[numthreads(7, 1, 1)]
void main(uint3 DTID : SV_GroupThreadID)
{
    thisMember.magnitude_start = (thisMember.magnitude_start + thisMember.magnitude_end) * 0.5f;
    thisMember.magnitude_end =  thisMember.magnitude_start;
    
}