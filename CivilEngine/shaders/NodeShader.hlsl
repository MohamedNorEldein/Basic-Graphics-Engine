/*
* each member has the start and end index of the start and end node
* the node index start from 0 to number of nodes - 1, then its followed by the support index + number of nodes
* [0 -> n - 1] => nodes , [n -> n + m - 1] => supports
*/
/*
* to solve for a memeber around a node we do the node equillibrium equation:
* sum Fx = 0;	sum Fy = 0;		sum Fz = 0; 
* we do it as F1 + F2 + F3 +.... = 0
* then f1 * e1 + f2 * e2 + f3 * e3 ..... = 0; where ei is the direction vector of the member as the node is the start
* if we dot the equation with e1 then:
* f1 + f2 * e1 * e2 + f3 * e1 * e3 .... = 0
* f1 = - (f2 * e1 * e2 + ....)
* 
* that method is converging if and only if the matrix is not diagonally dominant which is if only if the structure is geometrically staple.
* 
* at the boundries the last equation can not be applied cause the rections are perbendicular thus its not gonna be convergant 
* but it is always stable.
* so we do shifted dot product
* 
* f1 = - (f2 + f3 * e2 * e3 ....) / (e1 * e2)
* 
*/

#include "TrussTypes.hlsl"

float getMagnitude(uint vertexIndex, uint memberIndex)
{
    return (mArray[memberIndex].magnitude_start + mArray[memberIndex].magnitude_end ) * 0.5f;

}

float3 getdirection(uint vertexIndex, uint memberIndex)
{
	if (vertexIndex == mArray[memberIndex].start)
		return mArray[memberIndex].direction;
	return -mArray[memberIndex].direction;
}

void setMagnitude(uint vertexIndex, uint memberIndex, float value)
{
    if (mArray[memberIndex].start == vertexIndex)
    {
        mArray[memberIndex].magnitude_start = (value );
        return;
    }
    mArray[memberIndex].magnitude_end = (value);

   
}

void solveMember(uint vertexIndex, uint memberIndex)
{    
	float a = 0;
	uint j = 0;
    float3 dir = getdirection(vertexIndex, memberIndex);
    
	for (uint i = 0; i < vArray[vertexIndex].num; ++i)
	{
		j = vArray[vertexIndex].memberIndecies[i];
		if (j != memberIndex)
		{
			a += dot( getdirection(vertexIndex, j), dir) * getMagnitude(vertexIndex, j);
		} 
	}
	a += dot(vArray[vertexIndex].externalForce, dir);
    setMagnitude(vertexIndex, memberIndex, -a );
}

// the index of support[i] as vertex = [i + n] 

void solveMemberR(uint vertexIndex, uint memberIndex)
{
    float a = { 0 };
    uint j = 0;
    float3 memberDirection = getdirection(vertexIndex, memberIndex);

    for (uint i = 0; i < vArray[vertexIndex].num; ++i)
    {
        j = vArray[vertexIndex].memberIndecies[i];
        if (j != memberIndex)
        {
            a += (dot(getdirection(vertexIndex, j), getdirection(vertexIndex, memberIndex))) * getMagnitude(vertexIndex, j);
        }
    }
    a += dot(vArray[vertexIndex].externalForce, getdirection(vertexIndex, memberIndex));

    j = 0;
    while (j < sArray[vArray[vertexIndex].supportIndex].reactionNum)
    {
        a += dot(memberDirection, sArray[vArray[vertexIndex].supportIndex].rDir[j]) * sArray[vArray[vertexIndex].supportIndex].r[j];
        j++;
    }
    setMagnitude(vertexIndex, memberIndex, -a);
}

void solveReaction(uint vertexIndex, uint k)
{

    float a = { 0 };
    uint j = 0;
    sArray[vArray[vertexIndex].supportIndex].rDir[k];

    for (uint i = 0; i < vArray[vertexIndex].num; ++i)
    {
        j = vArray[vertexIndex].memberIndecies[i];
        a += dot(getdirection(vertexIndex, j), getdirection(vertexIndex, 0)) * getMagnitude(vertexIndex, j);
    }
    j = 0;
    while (j < sArray[vArray[vertexIndex].supportIndex].reactionNum)
    {
        if (j != k)
        {
            a += dot(getdirection(vertexIndex, 0), sArray[vArray[vertexIndex].supportIndex].rDir[j]) * sArray[vArray[vertexIndex].supportIndex].r[j];
        }

        j++;
    }

    sArray[vArray[vertexIndex].supportIndex].r[k] = -a / (dot(getdirection(vertexIndex, 0), sArray[vArray[vertexIndex].supportIndex].rDir[k]));
}


[numthreads(NODES_NUM, 1, 1)]
void main(uint3 DTID : SV_GroupThreadID)
{
    if (vArray[DTID.x].supportIndex==-1)
    {
        for (uint i = 0; i < vArray[DTID.x].num; ++i)
        {
            solveMember(DTID.x, vArray[DTID.x].memberIndecies[i]);
        }
    }
    else
    {
        uint i = 0;
        for (i = 0; i < sArray[vArray[DTID.x].supportIndex].reactionNum; ++i)
        {
            solveReaction(DTID.x, i);
        }

        for (i = 0; i < vArray[DTID.x ].num; ++i)
        {
            solveMemberR(DTID.x, vArray[DTID.x].memberIndecies[i]);
        }
		

    }
}
	
