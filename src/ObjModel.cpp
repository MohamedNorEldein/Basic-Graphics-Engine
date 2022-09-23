
#include "ObjModel.h"


DirectX::XMFLOAT3 GUItheta;
DirectX::XMFLOAT3 GUIpos;

ObjNode* selectedNode = 0;
bool nodeChange;

void ObjMesh::updateAPI(Graphics& gfx, TransformCBuffer& tr, const DirectX::XMMATRIX& PTrMat)
{
	using namespace DirectX;
	tr.update(gfx,

		XMMatrixScalingFromVector(XMLoadFloat3(&scale))
		*
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&theta))
		*
		XMMatrixTranslationFromVector(XMLoadFloat3(&pos))
		*
		PTrMat
	);
	/*
	XMFLOAT4X4 f;
	XMStoreFloat4x4(&f,
		PTrMat);
	printf("Matrix\n");
	printf("[%f, %f, %f, %f]\n", f._11, f._21, f._31, f._41);
	printf("[%f, %f, %f, %f]\n", f._12, f._22, f._32, f._42);
	printf("[%f, %f, %f, %f]\n", f._13, f._23, f._33, f._43);
	printf("[%f, %f, %f, %f]\n", f._14, f._24, f._34, f._44);
*/
}
void ObjMesh::_setGCLASS(GCLASS* gc)
{
	Drawable::_setGCLASS(gc);
}
void ObjMesh::setPos(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;
}
void ObjMesh::setRotation(float x, float y, float z) {
	theta.x = x;
	theta.y = y;
	theta.z = z;
}
void ObjMesh::setDiminsion(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
}
void ObjMesh::updatePos(float x, float y, float z) {
	pos.x += x;
	pos.y += y;
	pos.z += z;
}
void ObjMesh::updateRotation(float x, float y, float z) {
	theta.x += x;
	theta.y += y;
	theta.z += z;
}
void ObjMesh::updateDiminsion(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

ObjMesh::ObjMesh(const char* _name) :
	Drawable(), theta(0, 0, 0), scale(1.0, 1.0, 1.0), pos(0, 0, 0)
{
	memcpy(name, _name, strlen(name));
}
ObjMesh::ObjMesh(const char* _name, GCLASS& gclass) :
	Drawable(gclass), theta(0, 0, 0), scale(1.0, 1.0, 1.0), pos(0, 0, 0)
{
	memcpy(name, _name, strlen(name));
}
ObjMesh::ObjMesh(const char* _name, const char* gclassName) :
	Drawable(gclassName), theta(0, 0, 0), scale(1.0, 1.0, 1.0), pos(0, 0, 0)
{
	memcpy(name, _name, strlen(name));
}
ObjMesh::~ObjMesh() {
	std::cout << "delete objModel at :" << this << '\n';
};

ObjNode::ObjNode(const char* _name) :
	theta(0, 0, 0), pos(0, 0, 0), scale(1, 1, 1)
{
	memcpy(name, _name, strlen(_name));
}
ObjNode::ObjNode(const char* _name, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rotation) :
	theta(rotation), pos(pos), scale(1, 1, 1)
{
	memcpy(name, _name, strlen(_name));
}

void ObjNode::setTransilation(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;

}
void ObjNode::setRotation(float x, float y, float z) {
	theta.x = x;
	theta.y = y;
	theta.z = z;
}
void ObjNode::setDiminsion(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}
/* Add Mesh */
void ObjNode::attachMesh(const ObjMesh& mesh) {
	meshs.push_back((ObjMesh*)&mesh);
}
void ObjNode::attachMesh(ObjMesh* pmesh) {
	meshs.push_back(pmesh);
}
void ObjNode::attachMesh(const char* pmeshName) {
	meshs.push_back(new ObjMesh(pmeshName));
}

/* Add Node */
void ObjNode::addNode(ObjNode* pNode) {
	cheldren.push_back(pNode);
}
void ObjNode::addNode(const ObjNode& Node) {
	cheldren.push_back((ObjNode*)&Node);
}

/* Draw */
void ObjNode::Draw(Graphics& gfx, TransformCBuffer& tr, const DirectX::XMMATRIX& parentTransform) {
	using namespace DirectX;
	auto mat =
		XMMatrixScalingFromVector(XMLoadFloat3(&scale))
		*
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&theta))
		*
		XMMatrixTranslationFromVector(XMLoadFloat3(&pos))
		*
		parentTransform;

	for (auto pmesh : meshs) {
		pmesh->updateAPI(gfx, tr, mat);

		pmesh->Draw(gfx);
	}

	for (auto pnode : cheldren) {
		pnode->Draw(gfx, tr, mat);
	}
}

/* Gui Control */

void ObjNode::GuiControl() {
	ImGuiTreeNodeFlags f = 0;
	if (this->cheldren.empty()) {
		f = ImGuiTreeNodeFlags_Leaf;
	}

	if (ImGui::TreeNodeEx(name, f)) {
		if (ImGui::IsItemClicked())
		{
			selectedNode = this;
		}
		for (auto a : cheldren) {
			a->GuiControl();
		}

		ImGui::TreePop();
	}

}

ObjModel::ObjModel(Graphics& gfx, const char* src)
	:gfx(gfx)
{
	Assimp::Importer imp;
	auto pmodel = imp.ReadFile(src, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	/* Name */
	memcpy(name, pmodel->mName.C_Str(), pmodel->mName.length);

	/* Layout definesion */
	LayoutStrucure la;
	la.Append<DirectX::XMFLOAT3>("POSITION");
	la.Append<DirectX::XMFLOAT3>("NORMAL");

	aiMesh* pmesh;
	GCLASS* pgclass;

	/* creating common data*/
	VertexShader* vs = new VertexShader(gfx, L"shaders\\AssVertexShader.hlsl", false);
	PixelShader* ps = new PixelShader(gfx, L"shaders\\AssPixelShader.hlsl", false);
	InputLayout* Il = new InputLayout(gfx, la, vs->getpBlob());
	PrimativeTopology* pt = new PrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	tr = new TransformCBuffer(gfx);
	pcb = new PixelConstantBuffer(gfx, 1u, sizeof(COLOR));

	/* saving common data*/
	commonBindables.push_back(vs);
	commonBindables.push_back(ps);
	commonBindables.push_back(Il);
	commonBindables.push_back(pt);
	commonBindables.push_back(tr);
	commonBindables.push_back(pcb);

	/* creating root */
	root = new ObjNode("root", { 0,0,0 }, { 0,0,0 });

	for (int i = 0; i < pmodel->mNumMeshes; ++i) {
		/* creating mesh */
		pmesh = pmodel->mMeshes[i];

		vertexBufferData vbd(la, pmesh->mNumVertices);

		vbd.addData("POSITION", pmesh->mVertices);
		vbd.addData("NORMAL", pmesh->mNormals);

		unsigned short* IBData = new unsigned short[pmesh->mNumFaces * 3];

		for (int i = 0; i < pmesh->mNumFaces; i++)
		{
			IBData[3 * i] = pmesh->mFaces[i].mIndices[0];
			IBData[3 * i + 1] = pmesh->mFaces[i].mIndices[1];
			IBData[3 * i + 2] = pmesh->mFaces[i].mIndices[2];
		}


		pgclass = new GCLASS(pmesh->mName.C_Str());

		pgclass->AddBindable(new VertexBuffer(gfx, vbd));
		pgclass->AddBindable(new IndexBuffer(gfx, IBData, pmesh->mNumFaces * 3));

		meshsNames.push_back(new char[50]{ 0 });
		sprintf((char* const)meshsNames[meshsNames.size() - 1], "%s\000", pmesh->mName.C_Str());
	}
}

void ObjModel::Draw() {
	for (auto a : commonBindables) {
		a->bind(gfx);
	}
	root->Draw(gfx, *tr, DirectX::XMMatrixIdentity());
}

char GUI_MESH_NAME[CHAR_MAX], GUI_NODE_NAME[CHAR_MAX];

void  ObjModel::GuiControl() {


	if (ImGui::Begin("name")) {
		ImGui::Columns(4);
		//selectedNode = 0;

		root->GuiControl();

		if (selectedNode) {

			ImGui::NextColumn();
			/*	change parameters of the currently selected node */
			ImGui::Text("Position");
			ImGui::SliderFloat("position x", &GUIpos.x, -5000.0f, +5000.0f);
			ImGui::SliderFloat("position y", &GUIpos.y, -5000.0f, +5000.0f);
			ImGui::SliderFloat("position z", &GUIpos.z, 0.0f, +5000.0f);

			ImGui::Text("Rotation");
			ImGui::SliderAngle("rotation x", &GUItheta.x, -180.0f, +180.0f);
			ImGui::SliderAngle("rotation y", &GUItheta.y, -180.0f, +180.0f);
			ImGui::SliderAngle("rotation z", &GUItheta.z, -180.0f, +180.0f);


			if (ImGui::Button("reset", ImVec2(50, 25))) {
				GUItheta.x = { 0 };
				GUItheta.y = { 0 };
				GUItheta.z = { 0 };
				GUIpos.x = { 0 };
				GUIpos.y = { 0 };
				GUIpos.z = { 0 };

			}


			selectedNode->setTransilation(GUIpos.x, GUIpos.y, GUIpos.z);
			selectedNode->setRotation(GUItheta.x, GUItheta.y, GUItheta.z);
			/* attach mesh */

			ImGui::NextColumn();

			ImGui::Text("Attach mesh");
			ImGui::Combo("GCLASS_NAME :", &selectedmesh, meshsNames.data(), meshsNames.size());
			ImGui::InputText("##class name", GUI_MESH_NAME, CHAR_MAX);
			if (ImGui::Button("attach", ImVec2(50, 25))) {
				printf(GUI_MESH_NAME);

				ObjMesh* mesh = new ObjMesh(GUI_MESH_NAME, meshsNames[selectedmesh]);

				selectedNode->attachMesh(mesh);
			}


			/* attach node */

			ImGui::NextColumn();

			ImGui::Text("Attach Node");
			ImGui::InputText("##node name", GUI_NODE_NAME, CHAR_MAX);

			if (ImGui::Button("create", ImVec2(50, 25))) {
				printf(GUI_NODE_NAME);

				ObjNode* node = new ObjNode(GUI_NODE_NAME);

				selectedNode->addNode(node);
			}

		}
	}

	ImGui::End();
}


void ObjModel::setPos(float x, float y, float z) {
	root->setTransilation(x, y, z);
}

void ObjModel::setRotation(float x, float y, float z) {
	root->setRotation(x, y, z);

}

void ObjModel::setDiminsion(float x, float y, float z)
{
	root->setDiminsion(x, y, z);

}
