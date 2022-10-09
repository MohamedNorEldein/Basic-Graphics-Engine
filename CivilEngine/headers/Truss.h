#include "StaticSystem.h"

typedef struct _Node {
	DirectX::XMFLOAT3 pos, externalForce;
	UINT num, memberIndecies[5];
}_Node;

typedef struct _support 
{
	DirectX::XMFLOAT3 pos, externalForce;
	UINT num, memberIndecies[5];
	UINT reactionNum;
	DirectX::XMFLOAT3 rDir[3];
	float r[3];
}_support;

typedef struct _Member {
	DirectX::XMFLOAT3 direction;
	float magnitude;
	UINT start, end;
	bool ocupied;
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
	_Node{	{2.0f,	0.0f,	0.0f},	{0	,-10,0}	,4	,{0	,3,4,6,0}},	// 0
	_Node{	{1.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{0	,1,5,0,0}},	// 1
	_Node{	{3.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{2	,1,6,0,0}}	// 2
	};

	std::vector<_support> supports = 
	{
	_support{	{0.0f,	0.0f,	0.0f},	{0,0,0}	,2	,{5	,4,0,0,0}	,2	,{{0,1.0f,0},{1,0,0},{0,0,0}},	{0,0,0} },		// 3
	_support{	{4.0f,	0.0f,	0.0f},	{0,0,0}	,2	,{3	,2,0,0,0}	,1	,{{0,1.0f,0},{0,0,0},{0,0,0}},	{0,0,0}	}		// 4
	};

	std::vector<_Member> members = {
	{{0	,0	,0}	,0	,0	,1 ,0}, //0
	{{0	,0	,0}	,0	,1	,2 ,0}, //1
	{{0	,0	,0}	,0	,2	,4 ,0}, //2
	{{0	,0	,0}	,0	,4	,0 ,0}, //3
	{{0	,0	,0}	,0	,3	,0 ,0}, //4 
	{{0	,0	,0}	,0	,1	,3 ,0}, //5
	{{0	,0	,0}	,0	,2	,0 ,0}  //6
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
		nodes[startNodeIndex].memberIndecies[nodes[startNodeIndex].num] = members.size();
		nodes[startNodeIndex].num++;

		nodes[endNodeIndex].memberIndecies[nodes[endNodeIndex].num] = members.size();
		nodes[endNodeIndex].num++;

		members.push_back({ {},0,startNodeIndex ,endNodeIndex,0 });
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

	void printf(ComputeShaderOutput& memberBuffer) {
		{
			_Member* m = memberBuffer.read<_Member>(gfx);

			for (UINT i = 0; i < members.size(); ++i) {
				::printf("%u : %u -> %u : <%f %f %f> magnitude %f, %d\n",
					i, m[i].start, m[i].end,
					m[i].direction.x, m[i].direction.y, m[i].direction.z,
					m[i].magnitude,m[i].ocupied
				);
			}
			::printf("/**************/\n");
		}
	}

	void printf(ComputeShaderOutput& supportBuffer) {
		{
			_support* m = supportBuffer.read<_support>(gfx);
			for (UINT i = 0; i < supports.size(); ++i) {
				::printf("%u : f {%f %f %f},\n\t%u r1 %f,r2 %f r3 %f \n", i,
					m[i].externalForce.x, m[i].externalForce.y, m[i].externalForce.z,
					m[i].reactionNum,
					m[i].r[0],
					m[i].r[1],
					m[i].r[2]
				);
			}
			::printf("/**************/\n");
		}
	}


	void Analyse(UINT length = 50)
	{
		ComputeShaderOutput nodeBuffer(gfx, nodes, 0);
		ComputeShaderOutput memberBuffer(gfx, members, 1);
		ComputeShaderOutput supportBuffer(gfx, supports, 2);

		nodeBuffer.bind(gfx);
		memberBuffer.bind(gfx);
		supportBuffer.bind(gfx);

		PreMemberShader->bind(gfx);
		gfx.getcontext()->Dispatch(1, 1, 1);
		printf(memberBuffer);
		
		
		for (size_t i = 0; i < length; i++)
		{
			NodeShader->bind(gfx);
			gfx.getcontext()->Dispatch(1, 1, 1);
			//memberShader->bind(gfx);
			//gfx.getcontext()->Dispatch(1, 1, 1);
		}
		printf(memberBuffer);
		printf(supportBuffer);
		
	}


};

