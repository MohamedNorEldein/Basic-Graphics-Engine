
#include "ObjModel.h"
#include <string>


DirectX::XMFLOAT3 GUItheta;
DirectX::XMFLOAT3 GUIpos;
DirectX::XMFLOAT3 GUIScale;

ObjNode* selectedNode = 0;
ObjModel* selectedModel = 0;
bool nodeChange;

std::vector<ObjModel*> AllModels;


const wchar_t* wstr(const char* str, size_t length) {

	wchar_t* _wstr = new wchar_t(length);
	for (int i = 0; i < length; ++i) {
		_wstr[i] = wchar_t(str[i]);
	}
	wprintf(_wstr);
	return _wstr;
}

int printf(const DirectX::XMMATRIX& mat) {
	using namespace DirectX;
	XMFLOAT4X4 f;
	XMStoreFloat4x4(&f,
		mat);
	printf("Matrix\n");
	printf("[%f, %f, %f, %f]\n", f._11, f._21, f._31, f._41);
	printf("[%f, %f, %f, %f]\n", f._12, f._22, f._32, f._42);
	printf("[%f, %f, %f, %f]\n", f._13, f._23, f._33, f._43);
	printf("[%f, %f, %f, %f]\n", f._14, f._24, f._34, f._44);
	return 0;
}

int printf(const DirectX::XMVECTOR& v) {
	using namespace DirectX;
	XMFLOAT4 f;
	XMStoreFloat4(&f,
		v);
	printf("vector\n");
	printf("<%f, %f, %f, %f>\n", f.x, f.y, f.z, f.w);

	return 0;
}

void ObjMesh::updateAPI(Graphics& gfx, TransformCBuffer& tr, DirectX::XMMATRIX PTrMat)
{
	using namespace DirectX;
	tr.update(gfx, PTrMat);
	//tr.update(gfx, DirectX::XMMatrixIdentity());

}

ObjMesh::ObjMesh(const char* _name) :
	Drawable()
{
	memcpy(name, _name, strlen(name));
}

ObjMesh::~ObjMesh()
{
	printf("delete mesh %s at %x \n", name, this);
};

ObjNode::ObjNode(const char* _name)
{
	memcpy(name, _name, strlen(_name));
}


ObjNode::ObjNode(aiNode* nodeptr, ObjModel* GrandParent) :
	transformation((const float*)&nodeptr->mTransformation.a1)
{
	memcpy(name, nodeptr->mName.C_Str(), nodeptr->mName.length);

	transformation = DirectX::XMMatrixTranspose(transformation);
	ObjMesh* mesh = 0;
	for (int i = 0; i < nodeptr->mNumMeshes; ++i) {
		mesh = GrandParent->AllMeshs[nodeptr->mMeshes[i]];
		meshs.push_back(mesh);
		printf("node %s : adding mesh %s\n", name, mesh->name);
	}

	for (int i = 0; i < nodeptr->mNumChildren; ++i) {
		children.push_back(new ObjNode(nodeptr->mChildren[i], GrandParent));
	}
}

ObjNode::~ObjNode()
{
	printf("delete node : '%s'\n", name);

	for (auto a : children) {
		delete a;
	}
}

void ObjNode::setTransilation(float x, float y, float z) {
	using namespace DirectX;
	transformation *= XMMatrixTranslation(x, y, z);

}
void ObjNode::setRotation(float x, float y, float z) {
	using namespace DirectX;
	transformation *= XMMatrixRotationRollPitchYaw(x, y, z);

}
void ObjNode::setDiminsion(float x, float y, float z)
{
	using namespace DirectX;
	transformation *= XMMatrixScaling(x, y, z);
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
	children.push_back(pNode);
}
void ObjNode::addNode(const ObjNode& Node) {
	children.push_back((ObjNode*)&Node);
}

/* Draw */
void ObjNode::Draw(Graphics& gfx, TransformCBuffer& tr, DirectX::XMMATRIX parentTransform) {
	using namespace DirectX;
	auto mat = transformation * parentTransform;

	for (auto pmesh : meshs) {
		pmesh->updateAPI(gfx, tr, mat);

		pmesh->Draw(gfx);
	}

	for (auto pnode : children) {
		pnode->Draw(gfx, tr, mat);
	}
}

/* Gui Control */

void ObjNode::GuiControl() {
	ImGuiTreeNodeFlags f = 0;
	if (this->children.empty()) {
		f = ImGuiTreeNodeFlags_Leaf;
	}

	if (ImGui::TreeNodeEx(name, f)) {
		if (ImGui::IsItemClicked())
		{
			selectedNode = this;
		}
		for (auto a : children) {
			a->GuiControl();
		}
		ImGui::TreePop();
	}

}

struct ModelMaterialCbufferData
{
	__declspec(align(16)) aiColor4D DiffuseColor;
	__declspec(align(16)) aiColor4D AmpientColor;
	__declspec(align(16)) aiColor4D SpecularColor;
};


void ObjModel::loadModelFromFile(const std::string& folderName, const std::string& fileName)
{
	Assimp::Importer imp;

	auto pmodel = imp.ReadFile((folderName + "\\" + fileName).c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	printf(imp.GetErrorString());
	if (pmodel == 0) {
		throw std::exception("unable to open file");
	}


	/* Name */
	if (pmodel->mName.C_Str()[0] != 0) {
		memcpy(name, pmodel->mName.C_Str(), pmodel->mName.length);
	}
	else {
		memcpy(name, "model",sizeof("model"));
	}
	/* Layout definesion */
	LayoutStrucure  la2;

	la2.Append<DirectX::XMFLOAT3>("POSITION");
	la2.Append<DirectX::XMFLOAT3>("NORMAL");
	la2.Append<DirectX::XMFLOAT3>("TEXCOORD");

	aiMesh* pmesh;

	/* creating common data*/

	// textured
	VertexShader* vsT = new VertexShader(gfx, L"GraphicEngine\\shaders\\AssVertexShader.hlsl", false);
	PixelShader* psT = new PixelShader(gfx, L"GraphicEngine\\shaders\\AssPixelShader.hlsl", false);
	InputLayout* IlT = new InputLayout(gfx, la2, vsT->getpBlob());

	PrimativeTopology* pt = new PrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	tr = new TransformCBuffer(gfx);
	pcb = new RawCBuffer(gfx, 1u, sizeof(COLOR), PIXEL_SHADER_STAGE);

	printf("\n");
	/* saving common data*/
	commonBindables.insert(pt);
	commonBindables.insert(tr);
	commonBindables.insert(pcb);

	/* loading aiMeshs */
	for (int i = 0; i < pmodel->mNumMeshes; ++i) {
		/* creating objmesh */
		pmesh = pmodel->mMeshes[i];

		unsigned short* IBData = new unsigned short[pmesh->mNumFaces * 3];

		for (int i = 0; i < pmesh->mNumFaces; i++)
		{
			IBData[3 * i] = pmesh->mFaces[i].mIndices[0];
			IBData[3 * i + 1] = pmesh->mFaces[i].mIndices[1];
			IBData[3 * i + 2] = pmesh->mFaces[i].mIndices[2];
		}

		std::printf("loading mesh %s\n", (pmesh->mName.C_Str()));
		AllMeshs.push_back(new ObjMesh(pmesh->mName.C_Str()));



		vertexBufferData vbd(la2, pmesh->mNumVertices);
		vbd.addData("TEXCOORD", pmesh->mTextureCoords[0]);
		vbd.addData("POSITION", pmesh->mVertices);
		vbd.addData("NORMAL", pmesh->mNormals);
		auto material = pmodel->mMaterials[pmesh->mMaterialIndex];

		auto data = (float*)vbd.Data();
		
		aiString str;
		ModelMaterialCbufferData m;

		for (UINT i = aiTextureType_DIFFUSE; i < aiTextureType_EMISSIVE; i += 1)
		{
			if( material->GetTextureCount((aiTextureType)i) > 0u) 
			{
				material->GetTexture((aiTextureType)i, 0, &str);
				std::string c = folderName + "\\" + str.C_Str() + '\000';
				mbstowcs(_fileName, c.c_str(), c.size());
				wprintf(L"tetxture %s\n", _fileName);
				AllMeshs.back()->AddBindable(new Texture(gfx, _fileName, i - 1));
			}
			
		}

		AllMeshs.back()->AddBindable(new Sampler(gfx, 0u));
		AllMeshs.back()->AddBindable(new VertexBuffer(gfx, vbd));
		AllMeshs.back()->AddBindable(new IndexBuffer(gfx, IBData, pmesh->mNumFaces * 3));
		AllMeshs.back()->AddBindable(IlT->AddRefrance());
		AllMeshs.back()->AddBindable(vsT->AddRefrance());
		AllMeshs.back()->AddBindable(psT->AddRefrance());

		meshsNames.push_back(pmesh->mName.C_Str());
		sprintf((char*)meshsNames[meshsNames.size() - 1], "%s\000", pmesh->mName.C_Str());

	}


	/* creating Nodes using the common data recursively */
	root = new ObjNode(pmodel->mRootNode, this);


}

ObjModel* ObjModel::copy()
{
	/*
	* a copy algorithim using breadth firts algo
	*/
	return nullptr;
}

ObjModel::ObjModel(Graphics& gfx, const std::string& folderName, const std::string& fileName)
	:gfx(gfx), scale(1), rot(0, 0, 0), pos(0, 0, 0)
{
	loadModelFromFile(folderName, fileName);
}

ObjModel::ObjModel(Graphics& gfx)
	:gfx(gfx), scale(1), rot(0, 0, 0), pos(0, 0, 0)
{
}

ObjModel::~ObjModel()
{
	printf("\n\ndelete model %s\n", name);
	printf("delete common bindables");
	//........ member data deletion
	commonBindables.Release();

	printf("\ndelete Connected nodes ............................\n");
	//...........nodes
	delete root;


	printf("\ndelete Conected meshes .............................\n");

	//...........meshes
	for (auto a : AllMeshs) {
		delete a;
	}



}

void ObjModel::Draw() {
	commonBindables.bind(gfx);
	using namespace DirectX;
	root->Draw(gfx, *tr,
		DirectX::XMMatrixScaling(scale, scale, scale)
		*
		DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z)
		*
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)
	);
}

char GUI_MESH_NAME[CHAR_MAX], GUI_NODE_NAME[CHAR_MAX];

void  ObjModel::GuiControl() {
	selectedModel = this;
	root->GuiControl();
}


void ObjModel::setPos(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;

}

void ObjModel::setRotation(float x, float y, float z) {
	rot.x = x;
	rot.y = y;
	rot.z = z;

}

void ObjModel::setDiminsion(float x)
{
	scale = x;
}
char folder[100] = { 0 };
char file[20] = { 0 };

void ModelsGuiControl(Graphics& gfx)
{
	if (ImGui::Begin("name", nullptr, (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))) {
		ImGui::Text("Add Model ");

		ImGui::InputText("folder name", folder, 100);
		ImGui::InputText("file name", file, 20);
		if (ImGui::Button("attach", ImVec2(20, 20))) {
			try {
				AllModels.push_back(new ObjModel(gfx, folder, file));
			}
			catch (std::exception& e) {
				cout(e.what());
			}
			folder[0] = '\000';
			file[0] = '\000';

		}

		ImGui::Columns(2);

		for (auto a : AllModels) {
			a->GuiControl();
		}

		ImGui::NextColumn();
		if (selectedNode != 0) {
			ImGui::Text("Node Name %s", selectedNode->getName());
			for (auto a : selectedNode->meshs) {
				ImGui::Text("Mesh Name %s", a->getName());
			}

			//ImGui::NextColumn();
			/*	change parameters of the currently selected node */

			ImGui::Text("Position");
			ImGui::SliderFloat("position x", &selectedModel->pos.x, -500.0f, +500.0f);
			ImGui::SliderFloat("position y", &selectedModel->pos.y, -500.0f, +500.0f);
			ImGui::SliderFloat("position z", &selectedModel->pos.z, -500.0f, +500.0f);

			ImGui::Text("Rotation");
			ImGui::SliderAngle("rotation x", &selectedModel->rot.x, -180.0f, +180.0f);
			ImGui::SliderAngle("rotation y", &selectedModel->rot.y, -180.0f, +180.0f);
			ImGui::SliderAngle("rotation z", &selectedModel->rot.z, -180.0f, +180.0f);

			ImGui::Text("Scale");
			ImGui::SliderFloat("Scale   ", &selectedModel->scale, -100.0f, +100.0f);
			
		}
	}
	ImGui::End();

}

void DrawAllModels(Graphics& gfx) {
	for (auto a : AllModels) {
		a->Draw();
	}
}

