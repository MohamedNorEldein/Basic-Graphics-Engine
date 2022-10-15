
#define Export __declspec(dllexport)
#include "StaticSystem.h"


float atan(float num, float dem) {
	if (dem == 0)
		return MATH_PI / 4;
	return atan(num / dem);
}

StructuralSystem::StructuralSystem(Graphics& gfx) :
	gfx(gfx)
{
	void* nodes = nullptr, * supports = nullptr, * members = nullptr;
	UINT nodesNum = 0, memberNum = 0;

}

StructuralSystem::~StructuralSystem() {
	delete NodeShader;
	delete memberShader;
}

void StructuralSystem::bindNodeShader(ComputeShader* shader)
{
	NodeShader = shader;
}

void StructuralSystem::bindMemberShader(ComputeShader* shader)
{
	memberShader = shader;
}

void StructuralSystem::bindPreMemberShader(ComputeShader* shader)
{
	PreMemberShader = shader;
}

void StructuralSystem::bindNodesData(ComputeShaderOutput* sr) {
	NodeData = sr;
}

void StructuralSystem::bindMembersData(ComputeShaderOutput* sr) {
	memberData = sr;
}

void StructuralSystem::bindSupportData(ComputeShaderOutput* sr)
{
	supportData = sr;
}

void StructuralSystem::Analyse(UINT length)
{
	NodeData->bind(gfx);
	memberData->bind(gfx);
	supportData->bind(gfx);

	PreMemberShader->bind(gfx);
	gfx.getcontext()->Dispatch(1, 1, 1);


	UINT l2 = sqrt(length);
	for (size_t i = 0; i < length; i++)
	{
		NodeShader->bind(gfx);
		gfx.getcontext()->Dispatch(1, 1, 1);

		memberShader->bind(gfx);
		gfx.getcontext()->Dispatch(1, 1, 1);
	}


}

Structure1D::Structure1D(Graphics& gfx) :
	StructuralSystem(gfx)
{
}

void Structure1D::push(const Node& _data)
{
	nodes.push_back(_data);
}

void Structure1D::push(const Member& _data)
{
	members.push_back(_data);
}

void Structure1D::push(const Support& _data)
{
	supports.push_back(_data);
}

void Structure1D::push(const std::vector<Node>& _nodes)
{
	nodes = _nodes;
}

void Structure1D::push(const std::vector<Member>& _members)
{
	members = _members;
}

void Structure1D::push(const std::vector<Support>& _supports)
{
	supports = _supports;
}

void Structure1D::push(const wchar_t* shaderFileName, const char* name)
{
	shaders.push_back(new ComputeShader(gfx, shaderFileName, false));
	shadersNames.push_back(name);
}

void Structure1D::bindNodeShader(const char* shader) {
	StructuralSystem::bindNodeShader(shaders[shadersNames.find(shader)]);
}

void Structure1D::bindMemberShader(const char* shader) {
	StructuralSystem::bindMemberShader(shaders[shadersNames.find(shader)]);
}

void Structure1D::bindPreMemberShader(const char* shader) {
	StructuralSystem::bindPreMemberShader(shaders[shadersNames.find(shader)]);
}

#define START_NODE nodes[members[i].start]
#define END_NODE nodes[members[i].end]


void Structure1D::processData()
{
	Node* n = nodes.data();

	size_t i = 0;
	for (i = 0; i < nodes.size(); i++)
	{
		cubes.emplace_back(Cube(gfx));
		cubes.back().setPos(scale * n[i].pos.x, scale * n[i].pos.y, scale * n[i].pos.z);
		cubes.back().setDiminsion(nodeScale, nodeScale, nodeScale);
		cubes.back().setColor(0, 1, 0);

		n->num = 0;
		n->ocubied = 0;

	}

	for (size_t i = 0; i < members.size(); i++)
	{
		/* Node Connection */

		START_NODE.memberIndecies[START_NODE.num] = i;
		START_NODE.num += 1;
		//		members
		END_NODE.memberIndecies[END_NODE.num] = i;
		END_NODE.num += 1;

		/* cubes data */
		cubes.emplace_back(Cube(gfx));
		cubes.back().setPos(
			scale * (n[members[i].start].pos.x + n[members[i].end].pos.x) / 2,
			scale * (n[members[i].start].pos.y + n[members[i].end].pos.y) / 2,
			scale * (n[members[i].start].pos.z + n[members[i].end].pos.z) / 2
		);
		float	dx = (n[members[i].start].pos.x - n[members[i].end].pos.x),
			dy = (n[members[i].start].pos.y - n[members[i].end].pos.y),
			dz = (n[members[i].start].pos.z - n[members[i].end].pos.z);
		float length = scale * sqrt(dx * dx + dy * dy + dz * dz) / 2;

		cubes.back().setDiminsion((length), 1, 1);
		cubes.back().setRotation(
			atan(dz, dy), atan(dz, dx), atan(dy, dx)
		);
	}

	
	NODES_NUM = nodes.size();
}

void Structure1D::bindData()
{
	StructuralSystem::bindNodesData(new ComputeShaderOutput(gfx, nodes, 0u));
	StructuralSystem::bindMembersData(new ComputeShaderOutput(gfx, members, 1));
	StructuralSystem::bindSupportData(new ComputeShaderOutput(gfx, supports, 2u));
}

void Structure1D::bindNodeShader(uint i) {
	StructuralSystem::bindNodeShader(shaders[i]);
}

void Structure1D::bindMemberShader(uint i) {
	StructuralSystem::bindMemberShader(shaders[i]);
}

void Structure1D::bindPreMemberShader(uint i) {
	StructuralSystem::bindPreMemberShader(shaders[i]);
}

void Structure1D::ShowData()
{
	if (ImGui::Begin("Data"))
	{
		if (ImGui::BeginTable("Members", 4))
		{
			ImGui::TableSetupColumn("Member Index");
			ImGui::TableSetupColumn("Start Node");
			ImGui::TableSetupColumn("End Node");
			ImGui::TableSetupColumn("Force");

			ImGui::TableHeadersRow();
			
			float a = 5, c;
			for (size_t i = 0; i < members.size(); i++)
			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%d", i);
				ImGui::TableNextColumn();

				ImGui::Text("%d", members[i].start);
				ImGui::TableNextColumn();

				ImGui::Text("%d", members[i].end);
				ImGui::TableNextColumn();

				ImGui::Text("%f", members[i].magnitude_start);

			}
			ImGui::TableNextRow();

		}
		ImGui::EndTable();

		if (ImGui::BeginTable("Nodes",8))
		{
			ImGui::TableSetupColumn("Node Index");
			ImGui::TableSetupColumn("pos x");
			ImGui::TableSetupColumn("pos y");
			ImGui::TableSetupColumn("pos z");

			ImGui::TableSetupColumn("F_ex x");
			ImGui::TableSetupColumn("F_ex y");
			ImGui::TableSetupColumn("F_ex z");

			ImGui::TableSetupColumn("Suport index");

			ImGui::TableHeadersRow();

			float a = 5, c;
			for (size_t i = 0; i < nodes.size(); i++)
			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%d", i);

				ImGui::TableNextColumn();
				ImGui::Text("%f", nodes[i].pos.x);
				ImGui::TableNextColumn();
				ImGui::Text("%f", nodes[i].pos.y);
				ImGui::TableNextColumn();
				ImGui::Text("%f", nodes[i].pos.z);

				ImGui::TableNextColumn();
				ImGui::Text("%f", nodes[i].externalForce.x);
				ImGui::TableNextColumn();
				ImGui::Text("%f", nodes[i].externalForce.y);
				ImGui::TableNextColumn();
				ImGui::Text("%f", nodes[i].externalForce.z);

				ImGui::TableNextColumn();
				ImGui::Text("%d", nodes[i].supportIndex);

			}

		}
		ImGui::EndTable();
	}
	ImGui::End();

}

void Structure1D::updateView() {
	Member* d = memberData->read<Member>(gfx);

	for (size_t i = 0; i < members.size(); i++)
	{
		//cubes[i].setColor(c - 0.5, 0, 0.5 - c);
		members[i].magnitude_start = d[i].magnitude_start;
		members[i].magnitude_end = d[i].magnitude_end;
	}
	//Node* m = nodes.data();
	Node* m = NodeData->read<Node>(gfx);
	Support* s = supportData->read<Support>(gfx);

}

void Structure1D::Draw(Graphics& gfx)
{
	for (auto& a : cubes) {
		a.Draw();
	}
}

void Structure1D::GUIcontrol() {
	static char buff1[CHAR_MAX]{ 0 };
	static char buff2[CHAR_MAX]{ 0 }, buff3[CHAR_MAX]{ 0 };

	static wchar_t d[CHAR_MAX]{ 0 };
	if (ImGui::Begin("Structure1D Analysis Data"))
	{
		ImGui::InputText("push Shader", buff1, CHAR_MAX);
		ImGui::InputText("push Shader name", buff2, CHAR_MAX);
		if (ImGui::Button("push shader", ImVec2(40, 20))) {
			mbstowcs(d, buff1, CHAR_MAX);
			push(d, buff2);
		}

		ImGui::InputText("push Data file", buff3, CHAR_MAX);
		if (ImGui::Button("push Data", ImVec2(40, 20))) {
			parseDataFromFile(buff3);
		
		}


	}
	ImGui::End();


	if (ImGui::Begin("Analyse")) {
		static int selected1, selected2, selected3;
		ImGui::Combo("bind node Shader", &selected1, shadersNames.data(), shadersNames.size());

		ImGui::Combo("bind member Shader", &selected2, shadersNames.data(), shadersNames.size());

		ImGui::Combo("bind premember Shader", &selected3, shadersNames.data(), shadersNames.size());


		if (ImGui::Button("Analyse")) {
			bindNodeShader(shadersNames[selected1]);
			bindMemberShader(shadersNames[selected2]);
			bindPreMemberShader(shadersNames[selected3]);

			processData();
			bindData();

			Analyse(50);
			updateView();
		}
	}
	ImGui::End();
	ShowData();

}

#define LineSize 1024

void Structure1D::ParseNode(char* line)
{
	Node n;
	sscanf(line, "N,%f,%f,%f,%f,%f,%f,%d"
		, &n.pos.x
		, &n.pos.y
		, &n.pos.z
		, &n.externalForce.x
		, &n.externalForce.y
		, &n.externalForce.z
		, &n.supportIndex
	);

	nodes.emplace_back(n);
}

void Structure1D::ParseMember(char* line)
{
	Member m;
	sscanf(line, "M,%u,%u", &m.start, &m.end);
	members.emplace_back(m);
}

void Structure1D::ParseSupport(char* line)
{
	Support s;
	sscanf(line, "S,%u", &s.reactionNum);

	switch (s.reactionNum)
	{
	case 1:
		sscanf(line, "S,%u,%f,%f,%f", &s.reactionNum
			, &s.rDir[0].x
			, &s.rDir[0].y
			, &s.rDir[0].z
		);
		break;

	case 2:
		sscanf(line, "S,%u,%f,%f,%f,%f,%f,%f", &s.reactionNum
			, &s.rDir[0].x
			, &s.rDir[0].y
			, &s.rDir[0].z
			, &s.rDir[1].x
			, &s.rDir[1].y
			, &s.rDir[1].z
		);
		break;

	case 3:
		sscanf(line, "S,%u,%f,%f,%f,%f,%f,%f,%f,%f,%f", &s.reactionNum
			, &s.rDir[0].x
			, &s.rDir[0].y
			, &s.rDir[0].z
			, &s.rDir[1].x
			, &s.rDir[1].y
			, &s.rDir[1].z
			, &s.rDir[2].x
			, &s.rDir[2].y
			, &s.rDir[2].z
		);
		break;
	}
	supports.emplace_back(s);

}

void Structure1D::PareseLine(char* line)
{
	switch (line[0])
	{
	case 'N':
		ParseNode(line);
		break;
	case 'M':
		ParseMember(line);
		break;
	case 'S':
		ParseSupport(line);
		break;

	default:
		break;
	}
}

void Structure1D::parseDataFromFile(const char* FileName)
{
	FILE* file = fopen(FileName, "r");
	char line[LineSize];

	while (fgets(line, LineSize, file))
	{
		PareseLine(line);
		printf(line);
	}

	for (size_t i = 0; i < nodes.size(); i++)
	{
		printf("node %d"" pos %f %f %f"" external forces %f %f %f support index %d\n"
			"\t num of nodes %u:	%u %u %u %u %u \n"
			, i
			, nodes[i].pos.x
			, nodes[i].pos.y
			, nodes[i].pos.z
			, nodes[i].externalForce.x
			, nodes[i].externalForce.y
			, nodes[i].externalForce.z
			, nodes[i].supportIndex
			, nodes[i].num
			, nodes[i].memberIndecies[0]
			, nodes[i].memberIndecies[1]
			, nodes[i].memberIndecies[2]
			, nodes[i].memberIndecies[3]
			, nodes[i].memberIndecies[4]
		);


		if (nodes[i].supportIndex != -1) {
			Support& a = supports[nodes[i].supportIndex];
			printf("\treactions num %d are %f %f %f"
				, a.reactionNum
				, a.r[0], a.r[1], a.r[2]
			);
		}
		printf("\n");
	}


}


