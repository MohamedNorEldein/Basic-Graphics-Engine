#pragma once

#include "ComputeShader.h"
#include "ShaderResource.h"


#include <dxf/drw_base.h>
#include <dxf/drw_classes.h>
#include <dxf/drw_entities.h>
#include <dxf/drw_header.h>
#include <dxf/drw_interface.h>
#include <dxf/drw_objects.h>
#include <dxf/libdwgr.h>
#include <dxf/libdxfrw.h>




#include "constantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "IndexBuffer.h"
#include "PrimativeTopology.h"
#include "Texture.h"
#include "Sampler.h"
#include "Drawable.h"

#ifdef _DEBUG
#pragma comment (lib, "D:/computer/directX/Project1/x64/Debug/libdxfrw")

#else

#pragma comment (lib, "D:/computer/directX/Project1/x64/Release/libdxfrw")

#endif // _DEBUG

class System
{
protected:
	
	ComputeShaderOutput* NodeData, * memberData, * supportData;
	ComputeShader* NodeShader, * memberShader, * PreMemberShader;
	Graphics& gfx;

public:
	System(Graphics& gfx);
	virtual ~System();

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

	~stringVector()
	{
	}

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

	

	void insert(const char* item) {

		if (size() == 0) {
			push_back(item);
			return;
		}
		UINT i = size();
		push_back(0);

		while ((i > 0) && strcmp(item, (*this)[i - 1]))
		{
			(*this)[i] = (*this)[i - 1];
			i--;
		}
		(*this)[i] = item;

	}

	void print() {
		for (auto a : (*this))
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
	uint num, memberIndecies[5];
};

struct Support
{
	float3 pos, externalForce;
	uint num, memberIndecies[5];
	uint reactionNum;
	float3 rDir[3];
	float r[3];
};

struct Member
{
	float3 direction;
	float magnitude;
	uint start, end;
	bool ocubied;
};

#define VertexNum 10

class Structure1D:
	public System,
	public Drawable
{
private:
	struct vertex {
		float x, y, z;
	};


	D3D11_INPUT_ELEMENT_DESC ied[1] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

private:
	// data holders;
	std::vector<void*> data;
	stringVector names;

	// shaders
	std::vector<ComputeShader*> shaders;
	stringVector shadersNames;

	// view data
	vertex vertecies[VertexNum];
	TransformCBuffer *tr;
public:
	Structure1D(Graphics& gfx) :
		System(gfx),Drawable() 
	{
		tr = new TransformCBuffer(gfx, 0);
		VertexShader* vs = new VertexShader(gfx, L"GraphicEngine\\shaders\\VertexShader.hlsl", false);
		AddBindable(new PrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST));
		AddBindable(vs);
		AddBindable(new PixelShader(gfx, L"GraphicEngine\\shaders\\PixelShader.hlsl", false));
		AddBindable(new InputLayout(gfx,ied,_countof(ied), vs->getpBlob()));
		AddBindable(tr);
	}

	~Structure1D() = default;

	void push(const char *name,std::vector<Node>* _data)
	{
		data.push_back(_data);
		names.push_back(name);
	}
	void push(const char* name, std::vector<Member>* _data)
	{
		data.push_back(_data);
		names.push_back(name);
	}
	void push(const char* name, std::vector<Support>* _data)
	{
		data.push_back(_data);
		names.push_back(name);
	}
	void push(const wchar_t* shaderFileName)
	{
		shaders.push_back(new ComputeShader(gfx, shaderFileName, false));
	}
	void bindNodeShader(const char* shader) {
		System::bindNodeShader(shaders[shadersNames.find(shader)]);
	}
	void bindMemberShader(const char* shader) {
		System::bindMemberShader(shaders[shadersNames.find(shader)]);
	}
	void bindPreMemberShader(const char* shader){
		System::bindPreMemberShader(shaders[shadersNames.find(shader)]);
	}
	void bindMembersData(const char* _data) {
	//	delete this->memberData;
		std::vector<Member>& ptr = *(std::vector<Member>*)data[names.find(_data)];
		System::bindMembersData(new ComputeShaderOutput(gfx,ptr , 1));
		std::vector<unsigned short> indecies;
		for (size_t i = 0; i < ptr.size(); i++)
		{
			indecies.push_back( ptr[i].start);
			indecies.push_back(ptr[i].end);
		}
		

		AddBindable(new IndexBuffer(gfx, indecies));
	}
	void bindNodesData(const char* _nodes, const char* _supports) {

		std::vector<Node>* ptr = (std::vector<Node>*)data[names.find(_nodes)];
		System::bindNodesData(new ComputeShaderOutput(gfx, *ptr, 0u));
		size_t i = 0;
		Node* pt = ptr->data();
		for (i = 0; i < ptr->size(); i++)
		{
			vertecies[i].x = pt[i].pos.x;
			vertecies[i].y = pt[i].pos.y;
			vertecies[i].z = pt[i].pos.z;

		}

		std::vector<Support>* ptr2 = (std::vector<Support>*)data[names.find(_supports)];
		System::bindNodesData(new ComputeShaderOutput(gfx, *ptr2, 2u));
		size_t n = i;

		Support* pt2 = ptr2->data();

		for (i; i < (ptr2->size() + n); i++)
		{
			vertecies[i].x = pt2[i-n].pos.x;
			vertecies[i].y = pt2[i-n].pos.y;
			vertecies[i].z = pt2[i-n].pos.z;

		}
		AddBindable(new VertexBuffer(gfx, vertecies,n+ ptr2->size()));
		
	}
	void Draw(Graphics& gfx)
	{
		tr->update(gfx, XMMatrixScaling(100, 100, 100));
		Drawable::Draw(gfx);
	}

	void GUIcontrol() {
		char *buff1 = new char[100]{0};
		char* buff2 = new char[100]{ 0 };

		if (ImGui::Begin("Structure1D"))
		{
			ImGui::Columns(2);

			ImGui::InputText("push Shader",buff1,100);
			ImGui::InputText("push Data", buff2, 100);


		}
		ImGui::End();

	}
};
