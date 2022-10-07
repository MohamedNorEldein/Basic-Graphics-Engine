
//#include "App.h"
#define _XM_NO_INTRINSICS_

#include "ComputePipeLine.h"
#include "ShaderResource.h"
#include "ComputeShader.h"

//#include "Truss.h"
/*
typedef struct Node {
	DirectX::XMFLOAT3 pos,externalForce;
	UINT num, memberIndecies[5];

}Node;

typedef struct Member {
	DirectX::XMFLOAT3 direction;
	float magnitude_start, magnitude_end;
	UINT start, end;
}Member;

std::vector<Node> vArray = {
	{	{0.0f,	0.0f,	0.0f},	{0	,5	,0}	,2	,{0	,4,0,0,0}},		//0
	{	{2.0f,	0.0f,	0.0f},	{0	,-10,0}	,4	,{0	,1,5,6,0}},		//1
	{	{4.0f,	0.0f,	0.0f},	{0	,5  ,0}	,2	,{1	,2,0,0,0}},		//2
	{	{1.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{4	,5,3,0,0}},		//3
	{	{3.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{2	,3,6,0,0}}		//4
};

std::vector < Member> mArray = {
	{{0	,0	,0}	,0	,0	,0	,1	}, //0
	{{0	,0	,0}	,0	,0	,1	,2	}, //1
	{{0	,0	,0}	,0	,0	,2	,4	}, //2
	{{0	,0	,0}	,0	,0	,4	,3	}, //3
	{{0	,0	,0}	,0	,0	,3	,0	}, //4 
	{{0	,0	,0}	,0	,0	,1	,3	}, //5
	{{0	,0	,0}	,0	,0	,4	,1	}  //6
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	
	{
		ComputePipeLine gfx;
		// node shader
		ComputeShader NS(gfx, L"GraphicEngine\\shaders\\NodeShader.hlsl",false);
		// member  shader
		ComputeShader MS(gfx, L"GraphicEngine\\shaders\\MemberShader.hlsl", false);
		ComputeShader PMS(gfx, L"GraphicEngine\\shaders\\preMemberShader.hlsl", false);


		ComputeShaderOutput<Vertex> vertecies(gfx, vArray, 0);
		ComputeShaderOutput<Member> members(gfx, mArray, 1);

		vertecies.bind(gfx);
		members.bind(gfx);

		// defining the member prop
		PMS.bind(gfx);
		gfx.getcontext()->Dispatch(1, 1, 1);

		Member* m = members.read(gfx);

		for (int i = 0; i < 7; i++) {
			printf("member %d -> %d : direction (%f, %f, %f)\n",
				m[i].start,m[i].end,
				m[i].direction.x, m[i].direction.y, m[i].direction.z
			);
		}
		printf("***********************************************\n");

		NS.bind(gfx);
		MS.bind(gfx);

		for (int i = 0; i < 100; ++i) {
			NS.bind(gfx);
			gfx.getcontext()->Dispatch(1, 1, 1);
			MS.bind(gfx);
			gfx.getcontext()->Dispatch(1, 1, 1);

		}
		

		m = members.read(gfx);
	
		for (int i = 0; i < 7; i++) {
			printf("member %d <%u -> %u>: magnitude  %f %f\n",
				i,
				m[i].start, m[i].end,
				m[i].magnitude_start, m[i].magnitude_end
				);
		}

	}
	std::cin.get();
	FreeConsole();
	return 0;
}
*/


/*
* each member has the start and end index of the start and end node
* the node index start from 0 to number of nodes - 1, then its followed by the support index + number of nodes
* [0 -> n - 1] => nodes , [n -> n + m - 1] => supports
*/

using namespace DirectX;

typedef  DirectX::XMVECTOR float3;
typedef unsigned int uint;

#define dot(v1,v2) (DirectX::XMVector3Dot(v1,v2)).vector4_f32[0]
#define normalize(v) DirectX::XMVector3Normalize(v)



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

std::vector<Node> vArray = {
	Node{	{2.0f,	0.0f,	0.0f},	{0	,-10,0}	,4	,{0	,1,5,6,0}},		// 0
	Node{	{1.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{4	,5,3,0,0}},		// 1
	Node{	{3.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{2	,3,6,0,0}}		// 2
};

std::vector<Support> sArray =
{
Support{	{0.0f,	0.0f,	0.0f},	{0,0,0}	,2	,{0	,4,0,0,0}	,0	,{{0,5.0f,0},{0,0,0},{0,0,0} }},		// 3
Support{	{4.0f,	0.0f,	0.0f},	{0,0,0}	,2	,{1	,2,0,0,0}	,0	,{{0,5.0f,0},{0,0,0},{0,0,0} }},		// 4
};

std::vector<Member> mArray = {
{{0	,0	,0}	,0	,0	,0	,1	}, //0
{{0	,0	,0}	,0	,0	,1	,2	}, //1
{{0	,0	,0}	,0	,0	,2	,4	}, //2
{{0	,0	,0}	,0	,0	,4	,0	}, //3
{{0	,0	,0}	,0	,0	,3	,0	}, //4 
{{0	,0	,0}	,0	,0	,1	,3	}, //5
{{0	,0	,0}	,0	,0	,2	,0	}  //6
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

void solveMember(uint vertexIndex, uint memberIndex)
{
	float a = 0;
	uint j = 0;
	for (uint i = 0; i < vArray[vertexIndex].num; ++i)
	{
		j = vArray[vertexIndex].memberIndecies[i];
		if (j != memberIndex)
		{
			a += (dot(getdirection(vertexIndex, j), getdirection(vertexIndex, memberIndex))) * getMagnitude(vertexIndex, j);
		}
	}
	a += dot(vArray[vertexIndex].externalForce, getdirection(vertexIndex, memberIndex));
	setMagnitude(vertexIndex, memberIndex, -a);
}

// the index of support[i] as vertex = [i + n] 

void solveMemberR(uint vertexIndex, uint memberIndex, uint num)
{
	float3 a = { 0 };
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
	setMagnitude(vertexIndex, memberIndex, -dot(a, getdirection(vertexIndex, memberIndex)));
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
	sArray[vertexIndex - num].reaction[k] = -dot(a, dir) * dir;
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
		for (i = 0; i < sArray[DTID - num].num; ++i)
		{
			solveMemberR(DTID, sArray[DTID - num].memberIndecies[i], num);
		}
		for (i = 0; i < sArray[DTID - num].reactionNum; ++i)
		{
			solveReaction(DTID, i, num);
		}

	}

}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	{
		
		App app;
		app.go();
		

		ComputePipeLine gfx;
		Truss tr(gfx);
		tr.attachMemberShader(L"CivilEngine\\shaders\\MemberShader.hlsl");

		tr.attachNodeShader(L"CivilEngine\\shaders\\NodeShader.hlsl");

		tr.attachPreMemberShader(L"CivilEngine\\shaders\\preMemberShader.hlsl");
		tr.Analyse(50);
		

	}
	std::cin.get();
	FreeConsole();
	return 0;
}


