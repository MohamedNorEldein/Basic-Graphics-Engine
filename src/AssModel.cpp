
#include "AssModel.h"

AssModel::vertex* AssModel::vbData;
unsigned short* AssModel::IBData;


 std::vector<D3D11_INPUT_ELEMENT_DESC> AssModel::ied = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
};


void AssModel::Generate(const char* modelSrc) {

	Assimp::Importer imp;
	auto pmodel = imp.ReadFile(modelSrc, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if (!pmodel) {
		std::cout << "model loaded failed\n";
		return;
	}
	auto pmesh = pmodel->mMeshes[0];

	vbData = new vertex[pmesh->mNumVertices];

	for (int i = 0; i < pmesh->mNumVertices; i++) {
		vbData[i] = {
			pmesh->mVertices[i].x,
			pmesh->mVertices[i].y,
			pmesh->mVertices[i].z,
			pmesh->mNormals[i].x,
			pmesh->mNormals[i].y,
			pmesh->mNormals[i].z,

		};

	}

	IBData = new unsigned short[pmesh->mNumFaces * 3];

	for (int i = 0; i < pmesh->mNumFaces; i++)
	{

		IBData[3 * i] = pmesh->mFaces[i].mIndices[0];
		IBData[3 * i + 1] = pmesh->mFaces[i].mIndices[1];
		IBData[3 * i + 2] = pmesh->mFaces[i].mIndices[2];

	}


	AddBindable(new IndexBuffer(gfx, IBData, pmesh->mNumFaces * 3), _Indexbuffer);
	AddBindable(new VertexBuffer(gfx, vbData, pmesh->mNumVertices));
}


void AssModel::Init(const char* modelSrc) {
	Generate(modelSrc);
	VertexShader* vs = new VertexShader(gfx, L"AssVertexShader.cso");

	AddBindable(vs);
	AddBindable(new PixelShader(gfx, L"AssPixelShader.cso"));

	AddBindable(new InputLayout(gfx, ied, vs->getpBlob()));

	setPrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


void AssModel::setColor(float r, float g, float b, float a) {
	pccb->update(gfx, color{ r, g, b });
}


AssModel::AssModel(Graphics& gfx)
	:Gobject(gfx)
{
	if (isNotInitialized()) {
		//Init("../../Models/monkey.obj");
		Init("D:\\computer\\directX\\Project1\\Models\\monkey.obj");

	}
	AddBindable(new TransformCBuffer(gfx,0u), _TransforCBuffer);
	pccb = new PixelConstantBuffer<color>(gfx, 1u);
	AddBindable(pccb);
}



