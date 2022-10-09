#include "StaticSystem.h"


StaticSystem::StaticSystem(ComputePipeLine& gfx) :
	gfx(gfx)
{
	void* nodes = nullptr, * supports = nullptr, * members = nullptr;
	UINT nodesNum = 0, memberNum = 0;

}

StaticSystem::~StaticSystem() {
	delete NodeShader;
	delete memberShader;
}


void StaticSystem::bindNodeShader(const wchar_t* shaderSrc)
{
	//	delete NodeShader;
	NodeShader = new ComputeShader(gfx, shaderSrc, false);
}

void StaticSystem::bindMemberShader(const wchar_t* shaderSrc)
{
	//delete memberShader;
	memberShader = new ComputeShader(gfx, shaderSrc, false);
}

void StaticSystem::bindPreMemberShader(const wchar_t* shaderSrc)
{
	//	delete PreMemberShader;
	PreMemberShader = new ComputeShader(gfx, shaderSrc, false);
}

void StaticSystem::bindNodesData(void* data, UINT num, size_t stride) {
	nodes = data;
	nodesNum = num;
	nodesSize = stride;
}

void StaticSystem::bindMembersData(void* data, UINT num, size_t stride) {
	members = data;
	memberNum = num;
	memberSize = stride;

}

void StaticSystem::bindSupportData(void* data, UINT num, size_t stride)
{
	supports = data;
	supportNum = 0;
	supportSize = stride;
}

void StaticSystem::Analyse(UINT length)
{
	ComputeShaderOutput nodeBuffer(gfx, nodes, nodesNum, nodesSize, 0);
	ComputeShaderOutput	memberBuffer(gfx, members, memberNum, memberSize, 1);
	ComputeShaderOutput supportBuffer(gfx, supports, supportNum, supportSize, 0);

	nodeBuffer.bind(gfx);
	memberBuffer.bind(gfx);
	supportBuffer.bind(gfx);

	PreMemberShader->bind(gfx);
	gfx.getcontext()->Dispatch(1, 1, 1);

	UINT l2 = sqrt(length);
	for (size_t i = 0; i < length; i++)
	{
		for (size_t i = 0; i < l2; i++)
		{
			NodeShader->bind(gfx);
			gfx.getcontext()->Dispatch(1, 1, 1);
		}
		memberShader->bind(gfx);
		gfx.getcontext()->Dispatch(1, 1, 1);
	}
}

