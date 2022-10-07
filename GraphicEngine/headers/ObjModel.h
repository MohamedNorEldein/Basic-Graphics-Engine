#pragma once
#include "Drawable.h"

#include "constantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "IndexBuffer.h"
#include "PrimativeTopology.h"
#include "Texture.h"
#include "Sampler.h"
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
/*
	 void loadMesh(Graphics& gfx,ObjModel* model,aiMesh* pmesh);
	 void loadMaterial();
	 */
public:
	ObjMesh(const char* name);

	~ObjMesh();

	friend class ObjNode;
	friend void ModelsGuiControl();
	friend void ModelsGuiControl(Graphics& gfx);

};

class ObjNode {
private:
		// stack
	char name[CHAR_MAX];
	DirectX::XMMATRIX transformation;

private:
	//heap
	std::vector<ObjNode * > children;

	// owned by the parent model
	std::vector<ObjMesh * > meshs;
public:
	ObjNode(const char* name);
	ObjNode(aiNode* nodeptr, ObjModel* GrandParent);
	~ObjNode();
	
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
	const char * const getName() {
		return name;
	}

	friend class ObjModel;
	friend void ModelsGuiControl();
	friend void ModelsGuiControl(Graphics& gfx);

};


class ObjModel
{
private:
	// stack data
	char name[50];
	wchar_t _fileName[1024];

	Graphics& gfx;
	TransformCBuffer* tr;
	//int selectedmesh;
	DirectX::XMFLOAT3 pos, rot;
	float scale;
private:
	// heap data
	ObjNode* root;
	BV commonBindables;
	std::vector<const char*> meshsNames;
	std::vector<ObjMesh*> AllMeshs;


private:
	void loadModelFromFile(const std::string& folderName, const std::string& fileName);

	ObjModel* copy();

public:
	RawCBuffer* pcb;

	ObjModel(Graphics& gfx, const std::string& folderName, const std::string& fileName);
	ObjModel(Graphics& gfx);

	~ObjModel();

	void Draw();
	void GuiControl();

	/* position */
	void setPos(float x, float y, float z);

	void setRotation(float x, float y, float z);

	void setDiminsion(float x);

	friend class ObjNode;
	friend class ObjMesh;
	friend void ModelsGuiControl(Graphics& gfx);
};
void ModelsGuiControl(Graphics &gfx);
void DrawAllModels(Graphics& gfx);