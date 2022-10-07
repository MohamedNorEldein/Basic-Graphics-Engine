/*
* each member has the start and end index of the start and end node
* the node index start from 0 to number of nodes - 1, then its followed by the support index + number of nodes
* [0 -> n - 1] => nodes , [n -> n + m - 1] => supports
*/

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


float getMagnitude(uint vertexIndex, uint memberIndex)
{
	if (vertexIndex == mArray[memberIndex].start)
		return mArray[memberIndex].magnitude_start;
	return mArray[memberIndex].magnitude_end; 
}


float3 getdirection(uint vertexIndex, uint memberIndex)
{
	if (vertexIndex == mArray[memberIndex].start)
		return mArray[memberIndex].direction;
	return -mArray[memberIndex].direction;
}

void setMagnitude(uint vertexIndex, uint memberIndex, float value)
{
	if (vertexIndex == mArray[memberIndex].start)
	{
		mArray[memberIndex].magnitude_start = value;
	}
	else
	{    
		mArray[memberIndex].magnitude_end = value;
	}
}

void solveMember(uint vertexIndex, uint memberIndex)
{    
	float a = 0;
	uint j = 0;
	for (uint i = 0; i < vArray[vertexIndex].num; ++i)
	{
		j = vArray[vertexIndex].memberIndecies[i];
		if (j != memberIndex)
		{
			a += dot( getdirection(vertexIndex, j), getdirection(vertexIndex, memberIndex)) * getMagnitude(vertexIndex, j);
		} 
	}
	a += dot(vArray[vertexIndex].externalForce, getdirection(vertexIndex, memberIndex));
	  setMagnitude(vertexIndex, memberIndex, -a);
}

// the index of support[i] as vertex = [i + n] 

void solveMemberR(uint vertexIndex, uint memberIndex, uint num)
{
	float a = 0;
	uint j = 0;
	float3 memberDirection = getdirection(vertexIndex, memberIndex);
	
	for (uint i = 0; i < sArray[vertexIndex - num].num; ++i)
	{
		j = sArray[vertexIndex - num].memberIndecies[i];
		if (j != memberIndex)
		{
			a += getdirection(vertexIndex, j) * getMagnitude(vertexIndex, j);
		}
	}
	j = 0;
	while (j < sArray[vertexIndex - num].reactionNum)
	{
		a += sArray[vertexIndex - num].reaction[j];
		j++;
	}
    setMagnitude(vertexIndex, memberIndex, - dot(a, getdirection(vertexIndex, memberIndex)));
}

void solveReaction(uint vertexIndex, uint k, uint num)
{
	float3 a = { 0, 0, 0 };
	uint j;
	float3 dir = normalize(sArray[vertexIndex - num].reaction[k]);

	for (uint i = 0; i < sArray[vertexIndex - num].num; ++i)
	{
		j = sArray[vertexIndex - num].memberIndecies[i];
		a += getdirection(vertexIndex, j) * getMagnitude(vertexIndex, j);
	}
	j = 0;
	while (j < sArray[vertexIndex - num].reactionNum)
	{
		a += sArray[vertexIndex - num].reaction[j];
		j++;
	}
	sArray[vertexIndex - num].reaction[k] = - dot(a, dir) * dir;
}


[numthreads(5, 1, 1)]
void main(uint3 DTID : SV_GroupThreadID)
{
	uint num = 3;
	if (DTID.x < num)
	{
		for (uint i = 0; i < vArray[DTID.x].num; ++i)
		{
			solveMember(DTID.x, vArray[DTID.x].memberIndecies[i]);
		}
	}
	
	else
	{
		uint i = 0;
		for (i = 0; i < sArray[DTID.x - num].num; ++i)
		{
			solveMemberR(DTID.x, sArray[DTID.x-num].memberIndecies[i],num);
		}
		for (i = 0; i < sArray[DTID.x - num].reactionNum; ++i)
		{
			solveReaction(DTID.x, i, num);
		}
		
	}
	
}