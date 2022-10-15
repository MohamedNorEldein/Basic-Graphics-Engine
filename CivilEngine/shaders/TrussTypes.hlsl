

struct Node
{
    float3 pos, externalForce;
    uint num, memberIndecies[5];
    int supportIndex; //	if it's a support what is its index of crossponding data -1 means not a support
    bool ocubied;

};

struct Support
{
    uint reactionNum;
    float3 rDir[3];
    float r[3];
};

struct Member
{
    float3 direction;
    float magnitude_start, magnitude_end;
    uint start, end;
    bool ocubied;
};

RWStructuredBuffer<Node> vArray : register(u0);
RWStructuredBuffer<Member> mArray : register(u1);
RWStructuredBuffer<Support> sArray : register(u2);
/*
cbuffer NUM : register(u0)
{
    uint NODES_NUM, MEMBERS_NUM;
};
*/

#define NODES_NUM 5
#define MEMBERS_NUM 7