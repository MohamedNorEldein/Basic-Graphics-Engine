#include "StaticSystem.h"


System::System(Graphics& gfx) :
	gfx(gfx)
{
	void* nodes = nullptr, * supports = nullptr, * members = nullptr;
	UINT nodesNum = 0, memberNum = 0;

}

System::~System() {
//	delete NodeShader;
//	delete memberShader;
}


void System::bindNodeShader(ComputeShader* shader)
{
	NodeShader = shader;
}

void System::bindMemberShader(ComputeShader* shader)
{
	memberShader = shader;
}

void System::bindPreMemberShader(ComputeShader* shader)
{
	PreMemberShader = shader;
}

void System::bindNodesData(ComputeShaderOutput* sr) {
	NodeData = sr;
}

void System::bindMembersData(ComputeShaderOutput* sr) {
	memberData = sr;
}

void System::bindSupportData(ComputeShaderOutput* sr)
{
	supportData = sr;
}

void System::Analyse(UINT length)
{
	NodeData->bind(gfx);
	memberData->bind(gfx);
	supportData->bind(gfx);

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

