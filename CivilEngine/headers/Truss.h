#pragma once
#include "Cube.h"
#include "ComputeShader.h"
#include "ShaderResource.h"

typedef struct _Node {
	DirectX::XMFLOAT3 pos, externalForce;
	UINT num, memberIndecies[5];
/*
public:
	_Node(DirectX::XMFLOAT3 pos) :pos(pos), num(0)
	{
	}

	_Node(DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT3& d1) :pos(pos), num(0)
	{
		externalForce = (d1);
	}
	*/
}_Node;

typedef struct _support 
{
	DirectX::XMFLOAT3 pos, externalForce;
	UINT num, memberIndecies[5];
	UINT reactionNum;
	DirectX::XMFLOAT3 reaction[3];
}_support;

typedef struct _Member {
	DirectX::XMFLOAT3 direction;
	float magnitude_start, magnitude_end;
	UINT start, end;
}_Member;


class Node :
	public Cube,
	private _Node
{
private:

public:
	Node(Graphics& gfx, DirectX::XMFLOAT3 pos) :_Node{}, Cube(gfx)
	{

	}


};


class Member :public Cube
{
private:
public:
};



class Truss
{
private:
	std::vector<_Node> nodes = {
	_Node{	{2.0f,	0.0f,	0.0f},	{0	,-10,0}	,4	,{0	,1,5,6,0}},		// 0
	_Node{	{1.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{4	,5,3,0,0}},		// 1
	_Node{	{3.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{2	,3,6,0,0}}		// 2
	};

	std::vector<_support> supports = 
	{
	_support{	{0.0f,	0.0f,	0.0f},	{0,0,0}	,2	,{0	,4,0,0,0}	,0	,{{0,5.0f,0},{0,0,0},{0,0,0} }},		// 3
	_support{	{4.0f,	0.0f,	0.0f},	{0,0,0}	,2	,{1	,2,0,0,0}	,0	,{{0,5.0f,0},{0,0,0},{0,0,0} }},		// 4
	};

	std::vector<_Member> members = {
	{{0	,0	,0}	,0	,0	,0	,1	}, //0
	{{0	,0	,0}	,0	,0	,1	,2	}, //1
	{{0	,0	,0}	,0	,0	,2	,4	}, //2
	{{0	,0	,0}	,0	,0	,4	,0	}, //3
	{{0	,0	,0}	,0	,0	,3	,0	}, //4 
	{{0	,0	,0}	,0	,0	,1	,3	}, //5
	{{0	,0	,0}	,0	,0	,2	,0	}  //6
	};

	ComputeShader* NodeShader, * memberShader, * PreMemberShader;
	ComputePipeLine& gfx;

private:
	DirectX::XMFLOAT3& getPos(UINT index)
	{
		if (index > nodes.size()) {
			return supports[index % nodes.size()].pos;
		}
		return nodes[index].pos;
	}

public:
	Truss(ComputePipeLine& gfx) :
		gfx(gfx), NodeShader(nullptr), memberShader(nullptr)
	{
	}

	~Truss() {
		delete NodeShader;
		delete memberShader;
	}

	void pushNode(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 externalForce)
	{
		nodes.push_back(_Node{ pos,externalForce,0,int {0}});
	}

	void pushSupport(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 ReactionDirection)
	{
		supports.push_back(_support{ pos,{},0,{0},1,ReactionDirection,{},{}});
	}

	void pushSupport(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 ReactionDirection1 , DirectX::XMFLOAT3 ReactionDirection2)
	{
		supports.push_back(_support{ pos,{},0,{0},2,ReactionDirection1,ReactionDirection2,{} });
	}

	void pushSupport(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 ReactionDirection1, DirectX::XMFLOAT3 ReactionDirection2, DirectX::XMFLOAT3 ReactionDirection3)
	{
		supports.push_back(_support{ pos,{},0,{0},3,ReactionDirection1,ReactionDirection2,ReactionDirection3 });
	}

	void pushMember(UINT startNodeIndex, UINT endNodeIndex)
	{
		using namespace DirectX;
		XMFLOAT3 dir;
		dir.x = getPos(endNodeIndex).x - getPos(startNodeIndex).x;
		dir.y = getPos(endNodeIndex).y - getPos(startNodeIndex).y;
		dir.z = getPos(endNodeIndex).z - getPos(startNodeIndex).z;

		members.push_back({ dir,0,0,startNodeIndex ,endNodeIndex });
	}

	void attachNodeShader(const wchar_t* shaderSrc)
	{
	//	delete NodeShader;
		NodeShader = new ComputeShader(gfx, shaderSrc, false);
	}

	void attachMemberShader(const wchar_t* shaderSrc)
	{
		//delete memberShader;
		memberShader = new ComputeShader(gfx, shaderSrc, false);
	}

	void attachPreMemberShader(const wchar_t* shaderSrc)
	{
	//	delete PreMemberShader;
		PreMemberShader = new ComputeShader(gfx, shaderSrc, false);
	}

	void printMembers(ComputeShaderOutput<_Member>& memberBuffer) {
		{
			_Member* m = memberBuffer.read(gfx);

			for (UINT i = 0; i < members.size(); ++i) {
				printf("%u : %u -> %u : <%f %f %f> magnitude %f, %f\n",
					i, m[i].start, m[i].end,
					m[i].direction.x, m[i].direction.y, m[i].direction.z,
					m[i].magnitude_start,m[i].magnitude_end
				);
			}
			printf("/**************/\n");
		}
	}

	void printSupport(ComputeShaderOutput<_support>& supportBuffer) {
		{
			_support* m = supportBuffer.read(gfx);
			for (UINT i = 0; i < supports.size(); ++i) {
				printf("%u : f {%f %f %f},\n\t%u r1 {%f %f %f},r2 {%f %f %f}, r3{%f %f %f} \n", i,
					m[i].externalForce.x, m[i].externalForce.y, m[i].externalForce.z,
					m[i].reactionNum,
					m[i].reaction[0].x, m[i].reaction[0].y, m[i].reaction[0].z,
					m[i].reaction[1].x, m[i].reaction[1].y, m[i].reaction[1].z,
					m[i].reaction[2].x, m[i].reaction[2].y, m[i].reaction[2].z
				);
			}
			printf("/**************/\n");
		}
	}


	void Analyse(UINT length = 50)
	{
		ComputeShaderOutput<_Node> nodeBuffer(gfx, nodes, 0);
		ComputeShaderOutput<_Member> memberBuffer(gfx, members, 1);
		ComputeShaderOutput<_support> supportBuffer(gfx, supports, 2);

		nodeBuffer.bind(gfx);
		memberBuffer.bind(gfx);
		supportBuffer.bind(gfx);

		PreMemberShader->bind(gfx);
		gfx.getcontext()->Dispatch(1, 1, 1);

		
		for (size_t i = 0; i < length; i++)
		{
			NodeShader->bind(gfx);
			gfx.getcontext()->Dispatch(1, 1, 1);
			memberShader->bind(gfx);
			gfx.getcontext()->Dispatch(1, 1, 1);
		}
		printMembers(memberBuffer);
		printSupport(supportBuffer);
		
	}

	void loadDataFromFile(const wchar_t* filePath) {
		FILE* file = _wfopen(filePath, L"r");
		wchar_t line[512];


		while (fgetws(line, 512, file))
		{
			//swscanf(line,"%c %f %f %f")
			if (line[0] = L'N')
			{
				float x, y, z, dx, dy, dz, r;
				swscanf(line, L"N <%f %f %f> <%f %f %f> %f", &x, &y, &z, &dx, &dy, &dz, &r);
			}

		}

	}
};

