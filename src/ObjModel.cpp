
#include "ObjModel.h"


DirectX::XMMATRIX ObjModel::GetTransform() {
	using namespace DirectX;
	return
		XMMatrixScalingFromVector(XMLoadFloat3(&scale))
		*
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&theta))
		*
		XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
}

void ObjModel::setPos(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void ObjModel::setRotation(float x, float y, float z) {
	theta.x = x;
	theta.y = y;
	theta.z = z;
} 

void ObjModel::setDiminsion(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void ObjModel::updatePos(float x, float y, float z) {
	pos.x += x;
	pos.y += y;
	pos.z += z;
}

void ObjModel::updateRotation(float x, float y, float z) {
	theta.x += x;
	theta.y += y;
	theta.z += z;
}

void ObjModel::updateDiminsion(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
}




GCLASS* GenerateClassFromFile(Graphics& gfx, const char* pathToSrc) {

	Assimp::Importer imp;
	auto pmodel = imp.ReadFile(pathToSrc, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if (!pmodel) {
		std::cout << "model loaded failed\n";
		return 0;
	}
	auto pmesh = pmodel->mMeshes[0];


	LayoutStrucure la;

	la.Append<DirectX::XMFLOAT3>("POSITION");
	la.Append<DirectX::XMFLOAT3>("NORMAL");

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

	
	GCLASS* pgclass = new GCLASS(pmesh->mName.C_Str());
	VertexShader* vs = new VertexShader(gfx, L"AssVertexShader.cso");
	pgclass->AddBindable(new VertexBuffer(gfx, vbd));
	pgclass->AddBindable(new IndexBuffer(gfx, IBData, pmesh->mNumFaces * 3));
	pgclass->AddBindable(vs);
	pgclass->AddBindable(new PixelShader(gfx, L"AssPixelShader.cso"));
	pgclass->AddBindable(new InputLayout(gfx, la, vs->getpBlob()));
	pgclass->AddBindable(new PrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	pgclass->AddBindable(new TransformCBuffer(gfx));

	return pgclass;
}
