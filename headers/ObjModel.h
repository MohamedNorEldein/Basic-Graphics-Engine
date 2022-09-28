#pragma once
#include "Drawable.h"

#include "constantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "CBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "IndexBuffer.h"
#include "PrimativeTopology.h"


#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#ifdef _DEBUG
#pragma comment (lib, "D:/computer/directX/Project1/x64/Debug/assimp-vc143-mt")

#else

#pragma comment (lib, "D:/computer/directX/Project1/x64/Release/assimp-vc143-mt")

#endif // _DEBUG

__declspec(align(16))
struct COLOR {
	float r, b, g;
};
class ObjModel;

class ObjMesh:
	public Drawable
{
private:
	char name[50];
private:
	void updateAPI(Graphics& gfx, TransformCBuffer& tr,DirectX::XMMATRIX PTrMat);
public:
	 const char* const getName() {
		 return name;
	 }

	 // void loadMesh();

public:
	ObjMesh(const char* name);

	~ObjMesh();

	friend class ObjNode;
};

class ObjNode {
private:
	
public :
	char name[CHAR_MAX];
	DirectX::XMMATRIX transformation;
	std::vector<ObjNode * > children;
	std::vector<ObjMesh * > meshs;
public:
	ObjNode(const char* name);
	ObjNode(aiNode* nodeptr, ObjModel* GrandParent);

	
	/* offset */
	
	void setTransilation(float x, float y, float z);

	void setRotation(float x, float y, float z);

	void setDiminsion(float x, float y, float z);
	

	/* Add Mesh */
	void attachMesh(const ObjMesh& mesh);
	void attachMesh(ObjMesh* pmesh);
	void attachMesh(const char* pmeshName);

	/* Add Node */
	void addNode(ObjNode* pNode);
	void addNode(const ObjNode& Node);

	/* Draw */
	void Draw(Graphics& gfx, TransformCBuffer& tr, DirectX::XMMATRIX parentTransform);

	/* Gui Control */
	void GuiControl();

};


class ObjModel 
{
private:
	ObjNode* root;
	char name[50];
	Graphics& gfx;
	TransformCBuffer* tr;
	std::vector<Bindable*> commonBindables;
	std::vector<const char *> meshsNames;
	std::vector<ObjMesh*> AllMeshs;
	int selectedmesh;
	DirectX::XMFLOAT3 pos, scale, rot;

private:
	void loadModelFromFile(const char* srcFileName);

public:
	PixelConstantBuffer* pcb;

	ObjModel(Graphics& gfx, const char* src);

	void Draw();
	void GuiControl();

	/* position */
	void setPos(float x, float y, float z);

	void setRotation(float x, float y, float z);

	void setDiminsion(float x, float y, float z);

	friend class ObjNode;
	
};


 