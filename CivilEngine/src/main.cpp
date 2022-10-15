#define Export extern "C"  __declspec(dllexport)

#include "PyWraper.h"
//#include "StaticSystem.h"
/*
#include "WindowClass.h"
#include "Light.h"
#include "ObjModel.h"
#include "cube.h"

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

typedef  DirectX::XMVECTOR float3;
typedef unsigned int uint;


struct Node
{
	float3 pos, externalForce;
	uint num, memberIndecies[5];
	int supportIndex;	//	if it's a support what is its index of crossponding data -1 means not a support
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


#define dot(v1,v2) (DirectX::XMVector3Dot(v1,v2)).vector4_f32[0]
#define normalize(v) DirectX::XMVector3Normalize(v)

std::vector<Node> vArray = {
	Node{	{2.0f,	0.0f,	0.0f},	{0	,-10,0}	,4	,{0	,3,4,6,0}, -1, 0},		// 0
	Node{	{1.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{0	,1,5,0,0}, -1, 0},		// 1
	Node{	{3.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{2	,1,6,0,0}, -1, 0},		//	2
	Node{	{0.0f,	0.0f,	0.0f},	{0	,0	,0}	,2	,{5	,4,0,0,0},	0, 0},
	Node{	{4.0f,	0.0f,	0.0f},	{0	,0	,0}	,2	,{3	,2,0,0,0},	1, 0}
};

std::vector<Support> sArray =
{
Support{	2u	,{{0,1.0f,0},{1.0,0,0},{0,0,0}},	{0.0,0.0,0} },
Support{	1	,{{0,1.0f,0},{0.0,0,0},{0,0,0}},	{0.0,0,0}},
};

std::vector<Member> mArray =
{
{{-0.447214	, 0.894427 	,0},0.0f ,0.0f ,0 ,1	}, //0
{{1.000000	, 0.000000 	,0},0.0f ,0.0f ,1 ,2	}, //1
{{0.447214	, -0.894427	,0},0.0f ,0.0f ,2 ,4	}, //2
{{-1.000000	, 0.000000 	,0},0.0f ,0.0f ,4 ,0	}, //3
{{1.000000	, 0.000000 	,0},0.0f ,0.0f ,3 ,0	}, //4
{{-0.447214	, -0.894427	,0},0.0f ,0.0f ,1 ,3	}, //5
{{-0.447214	, -0.894427	,0},0.0f ,0.0f ,2 ,0	}  //6
};

float getMagnitude(uint vertexIndex, uint memberIndex)
{
	return (mArray[memberIndex].magnitude_start + mArray[memberIndex].magnitude_end) * 0.5f;

}

float3 getdirection(uint vertexIndex, uint memberIndex)
{
	if (vertexIndex == mArray[memberIndex].start)
		return mArray[memberIndex].direction;
	return - mArray[memberIndex].direction;
}

void setMagnitude(uint vertexIndex, uint memberIndex, float value)
{
	if (mArray[memberIndex].start == vertexIndex)
	{
		mArray[memberIndex].magnitude_start = (value);
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
			a += dot(getdirection(vertexIndex, j), dir) * getMagnitude(vertexIndex, j);
		}
	}
	a += dot(vArray[vertexIndex].externalForce, dir);
	setMagnitude(vertexIndex, memberIndex, -a);
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

void func(uint DTID)
{
	if (vArray[DTID].supportIndex == -1)
	{
		for (uint i = 0; i < vArray[DTID].num; ++i)
		{
			solveMember(DTID, vArray[DTID].memberIndecies[i]);
		}
	}
	else
	{
		uint i = 0;
		for (i = 0; i < sArray[vArray[DTID].supportIndex].reactionNum; ++i)
		{
			solveReaction(DTID, i);
		}

		for (i = 0; i < vArray[DTID].num; ++i)
		{
			solveMemberR(DTID, vArray[DTID].memberIndecies[i]);
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


void func() {
	App app;
	app.go();
	//delete app;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	func();
	
	std::cin.get();
	FreeConsole();
	return 0;
}

Export
int main() 
{

	

	app = new App();
	app->go();
	delete app;

	
	return 0;
}

