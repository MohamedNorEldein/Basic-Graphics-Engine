#pragma once

#pragma once
#include "ComputeShader.h"
#include "ShaderResource.h"



class StaticSystem
{
private:
	
	struct {
		void* nodes, * supports, * members;
		UINT nodesNum, memberNum, supportNum;
		size_t nodesSize, memberSize, supportSize;
	};

	ComputeShader* NodeShader, * memberShader, * PreMemberShader;
	ComputePipeLine& gfx;

public:
	StaticSystem(ComputePipeLine& gfx);
	~StaticSystem();

	void bindNodeShader(const wchar_t* shaderSrc);
	void bindMemberShader(const wchar_t* shaderSrc);
	void bindPreMemberShader(const wchar_t* shaderSrc);

	void bindNodesData(void* data, UINT num, size_t stride);
	void bindMembersData(void* data, UINT num, size_t stride);
	void bindSupportData(void* data, UINT num, size_t stride);
	
	void Analyse(UINT length = 50);
};
