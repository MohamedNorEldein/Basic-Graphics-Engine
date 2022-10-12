
#include "App.h"
#define _XM_NO_INTRINSICS_
#include "StaticSystem.h"


int printf(const DirectX::XMMATRIX& mat) {
	using namespace DirectX;
	XMFLOAT4X4 f;
	XMStoreFloat4x4(&f,
		mat);
	printf("Matrix\n");
	printf("[%f, %f, %f, %f]\n", f._11, f._21, f._31, f._41);
	printf("[%f, %f, %f, %f]\n", f._12, f._22, f._32, f._42);
	printf("[%f, %f, %f, %f]\n", f._13, f._23, f._33, f._43);
	printf("[%f, %f, %f, %f]\n", f._14, f._24, f._34, f._44);
	return 0;
}

int printf(const DirectX::XMVECTOR& v) {
	using namespace DirectX;
	XMFLOAT4 f;
	XMStoreFloat4(&f,
		v);
	printf("vector\n");
	printf("<%f, %f, %f, %f>\n", f.x, f.y, f.z, f.w);

	return 0;
}





using namespace DirectX;

#define dot(v1,v2) (DirectX::XMVector3Dot(v1,v2)).vector4_f32[0]
#define normalize(v) DirectX::XMVector3Normalize(v)

/*
std::vector<Node> vArray = {
	Node{	{2.0f,	0.0f,	0.0f},	{0	,-10,0}	,4	,{0	,3,4,6,0}},		// 0
	Node{	{1.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{0	,1,5,0,0}},		// 1
	Node{	{3.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{2	,1,6,0,0}}
};

std::vector<Support> sArray =
{
Support{	{0.0f,	0.0f,	0.0f},	{0,0,0}	,2	,{5	,4,0,0,0}	,1	,{{0,1.0f,0},{1.0,0,0},{0,0,0}},	{5.0,1.0,0} },		// 3
Support{	{4.0f,	0.0f,	0.0f},	{0,0,0}	,2	,{3	,2,0,0,0}	,1	,{{0,1.0f,0},{0.0,0,0},{0,0,0}},	{5.0,0,0}	},		// 4
};

std::vector<Member> mArray =
{
{{-0.447214	, 0.894427 	,0}	,	0 ,0 ,1	}, //0
{{1.000000	, 0.000000 	,0}	,   0 ,1 ,2	}, //1
{{0.447214	, -0.894427	,0}	,   0 ,2 ,4	}, //2
{{-1.000000	, 0.000000 	,0}	,	0 ,4 ,0	}, //3
{{1.000000	, 0.000000 	,0}	,	0 ,3 ,0	}, //4 
{{-0.447214	, -0.894427	,0}	,   0 ,1 ,3	}, //5
{{-0.447214	, -0.894427	,0}	,	0 ,2 ,0	}  //6
};

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
	return - mArray[memberIndex].direction;
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

uint Dir(uint vertexIndex, uint memberIndex)
{
	float3 a = getdirection(vertexIndex, memberIndex);
	float b;
	uint i = 0;
	do
	{
		memberIndex = vArray[vertexIndex].memberIndecies[(i++) % vArray[vertexIndex].num];
		b = dot(getdirection(vertexIndex, memberIndex), a);

	} while (b > 0.1 && b <-0.1);
	return memberIndex;
}

void solveMember(uint vertexIndex, uint memberIndex)
{
	float a = 0;
	uint j = 0;
	float3 dir = getdirection(vertexIndex, memberIndex);
//	printf("%d\t", vArray[vertexIndex].memberIndecies[Dir(vertexIndex,memberIndex)]);
	for (uint i = 0; i < vArray[vertexIndex].num; ++i)
	{
		j = vArray[vertexIndex].memberIndecies[i];
		if (j != memberIndex)
		{
			a += dot(getdirection(vertexIndex, j), dir) * getMagnitude(vertexIndex, j);
		}
	}
	a += dot(vArray[vertexIndex].externalForce, dir);
	setMagnitude(vertexIndex, memberIndex, -a );
}

// the index of support[i] as vertex = [i + n] 

void solveMemberR(uint vertexIndex, uint memberIndex, uint num)
{

	float a = { 0 };
	uint j = 0;
	float3 memberDirection = getdirection(vertexIndex, memberIndex);

	for (uint i = 0; i < sArray[vertexIndex-num].num; ++i)
	{
		j = sArray[vertexIndex - num].memberIndecies[i];
		if (j != memberIndex)
		{
			a += (dot(getdirection(vertexIndex, j), getdirection(vertexIndex, memberIndex))) * getMagnitude(vertexIndex, j);
		}
	}
	a += dot(sArray[vertexIndex - num].externalForce, getdirection(vertexIndex, memberIndex));

	j = 0;
	while (j < sArray[vertexIndex - num].reactionNum)
	{
		a += dot(memberDirection, sArray[vertexIndex - num].rDir[j]) * sArray[vertexIndex - num].r[j];
		j++;
	}
	setMagnitude(vertexIndex, memberIndex, -a);
}

void solveReaction(uint vertexIndex, uint k, uint num)
{

	float a = { 0 };
	uint j = 0;
	sArray[vertexIndex - num].rDir[k];

	for (uint i = 0; i < sArray[vertexIndex - num].num; ++i)
	{
		j = sArray[vertexIndex - num].memberIndecies[i];
		a += dot(getdirection(vertexIndex, j), getdirection(vertexIndex, 0)) * getMagnitude(vertexIndex, j);
	}
	j = 0;
	while (j < sArray[vertexIndex - num].reactionNum)
	{
		if (j != k) 
		{
			a += dot(getdirection(vertexIndex, 0) ,sArray[vertexIndex - num].rDir[j]) * sArray[vertexIndex - num].r[j];
		}

		j++;
	}

	sArray[vertexIndex - num].r[k] = - a / (dot(getdirection(vertexIndex, 0), sArray[vertexIndex - num].rDir[k]));
	//printf("sArray[vertexIndex - num].r[%u] = - %f\n", k,-a);


}

void func(uint DTID)
{
	uint num = 3;
	if (DTID < num)
	{
		for (uint i = 0; i < vArray[DTID].num; ++i)
		{
			solveMember(DTID, vArray[DTID].memberIndecies[i]);
		}
	}
	
	else
	{
		uint i = 0;
		for (i = 0; i < sArray[DTID - num].reactionNum; ++i)
		{
			solveReaction(DTID, i, num);
		}

		for (i = 0; i < sArray[DTID - num].num ; ++i)
		{
			solveMemberR(DTID, sArray[DTID - num].memberIndecies[i], num);
		}
	}
	
}

void printMembers() {
	
		Member* m =mArray.data();

		for (UINT i = 0; i < mArray.size(); ++i) {
			printf("%u : %u -> %u : <%f %f %f> magnitude %f, %f\n",
				i, m[i].start, m[i].end,
				m[i].direction.vector4_f32[0], m[i].direction.vector4_f32[1], m[i].direction.vector4_f32[2],
				m[i].magnitude_start, m[i].magnitude_end
			);
		}
	
}

void printSupport() {
	{
		Support* m = sArray.data();
		for (UINT i = 0; i < sArray.size(); ++i) {
			printf("%u :\t%u r {%f %f %f} \n", i,
				m[i].reactionNum,
				m[i].r[0],
				m[i].r[1],
				m[i].r[2]
			);
		}
		printf("**************\n");
	}
}

#define thisMember mArray[i]


void Analyis() {
	printMembers();
	printSupport();
	for (size_t k = 0; k < 50; k++)
	{
		for (size_t i = 0; i < 5; i++)
		{
			func(i);
		}
	//	printMembers();
	//	printf("***********************************\n");
		for (size_t i = 0; i < 7; i++)
		{
			thisMember.magnitude_start = (thisMember.magnitude_start + thisMember.magnitude_end) * 0.5f;
			thisMember.magnitude_end = thisMember.magnitude_start;
		}
		
	}
	printMembers();
	printSupport();

}
*/

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	{
		
		App app;
		app.go();
		
		
		
	}
	std::cin.get();
	FreeConsole();
	return 0;
}



/*
	5.590172
   -5.000005
   -5.590170
	2.500003
	2.500003
   -5.590171
	5.590170
*/