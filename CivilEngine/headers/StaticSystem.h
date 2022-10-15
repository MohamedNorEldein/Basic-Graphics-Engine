#pragma once

#include "ComputeShader.h"
#include "ShaderResource.h"
#include "Cube.h"

#ifndef Export
#define Export extern "C" __declspec(dllimport)
#endif // Export


float atan(float num, float dem);
Export class StructuralSystem
{
protected:
	
	ComputeShaderOutput* NodeData, * memberData, * supportData;
	ComputeShader* NodeShader, * memberShader, * PreMemberShader;
	Graphics& gfx;

public:
	StructuralSystem(Graphics& gfx);
	virtual ~StructuralSystem();

	void bindNodeShader(ComputeShader * shader);
	void bindMemberShader(ComputeShader* shader);
	void bindPreMemberShader(ComputeShader* shader);

	void bindNodesData(ComputeShaderOutput*);
	void bindMembersData(ComputeShaderOutput*);
	void bindSupportData(ComputeShaderOutput*);
	
	void Analyse(UINT length = 50);
};

class stringVector :public std::vector<const char*>
{
private:


public:
	stringVector() :
		std::vector<const char*>()
	{
	}

	~stringVector() = default;

	UINT find(const char* type) 
	{
		for (UINT i = 0; i < this->size(); ++i)
		{
			if (strcmp((*this)[i], type) == 0) {
				return i;
			}
		}
		return 0;
	}

	void print() {
		for (auto& a : (*this))
		{
			printf("(%s)",  a);
		}
	}
};

using namespace DirectX;

typedef  DirectX::XMFLOAT3 float3;
typedef unsigned int uint;


struct Node
{
	float3 pos, externalForce;
	uint num, memberIndecies[5]{0};
	int supportIndex;	//	if it's a support what is its index of crossponding data -1 means not a support
	bool ocubied;

	Node() :
		num(0), supportIndex(-1), ocubied(false)
	{

	}

	Node(float x, float y, float z, float fx, float fy, float fz) :
		pos(x,y,z), externalForce(fx, fy,fz),num(0),supportIndex(-1),ocubied(false)
	{
	}

	Node(float x, float y, float z, float fx, float fy, float fz,int supportIndex) :
		pos(x, y, z), externalForce(fx, fy, fz), num(0), supportIndex(supportIndex), ocubied(false)
	{
	}

};

struct Support
{
	uint reactionNum;
	float3 rDir[3];
	float r[3];

	Support() 
	{
	}

	Support(float3 _rDir):
		reactionNum(1) 
	{
		rDir[0] = _rDir;
	}

	Support( float3 _rDir1, float3 _rDir2) :
		reactionNum(2)
	{
		rDir[0] = _rDir1;
		rDir[1] = _rDir2;
	}
	
	Support(float3 _rDir1, float3 _rDir2, float3 _rDir3) :
		reactionNum(3)
	{
		rDir[0] = _rDir1;
		rDir[1] = _rDir2;
		rDir[2] = _rDir3;
	}
};

struct Member
{
	float3 direction;
	float magnitude_start, magnitude_end;
	uint start, end;
	bool ocubied;

	Member(uint start,uint end):
		start(start), end(end),magnitude_start(0),magnitude_end(0),ocubied(0)
	{
	}

	Member():magnitude_start(0), magnitude_end(0), ocubied(0)
	{

	}

};

#define VertexNum 10

class Structure1D:
	public StructuralSystem
{
public:
	float scale = 100, nodeScale = 5;

public:
	// data holders;
	std::vector<Node> nodes;
	std::vector<Member> members;
	std::vector<Support> supports;
	//view data
	std::vector<Cube> cubes;
	// shaders
	std::vector<ComputeShader*> shaders;
	stringVector shadersNames;

	


	//
	uint NODES_NUM, MEMBERS_NUM;
	
private:

public:
	Structure1D(Graphics& gfx);
	~Structure1D() = default;
	void push(const Node& _data);
	void push(const Member& _data);
	void push(const Support& _data);

	void push(const std::vector<Node>& nodes);
	void push(const std::vector<Member>& members);
	void push(const std::vector<Support>& supports);

	void push(const wchar_t* shaderFileName, const char* name);
	/* bind by name */
	void bindNodeShader(const char* shader);
	void bindMemberShader(const char* shader);
	void bindPreMemberShader(const char* shader);
	void processData();
	void bindData();
	/* bind by index */
	void bindNodeShader(uint i);
	void bindMemberShader(uint i);
	void bindPreMemberShader(uint i);
	
	void ShowData();
	void updateView();
	void Draw(Graphics& gfx);
	void GUIcontrol();

	void ParseSupport(char* line);
	void ParseMember(char* line);
	void ParseNode(char* line);

	void PareseLine(char* line);
	void parseDataFromFile(const char* shaderFileName);

};
