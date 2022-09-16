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
struct color {
	float r, b, g;
};

class ObjModel :
	public Drawable
{
private:
	DirectX::XMFLOAT3 theta, scale, pos;
	Graphics& gfx;
public:
	PixelConstantBuffer<color> *pPCbuff;

public:
	DirectX::XMMATRIX GetTransform() override;

	void GuiControl() {

		if (ImGui::Begin("model")) {

			ImGui::SliderFloat3("camera Rotation:", (float*) & theta, -5.0f, +5.0f);

			if (ImGui::Button("reset", ImVec2(50, 25))) {
				theta.x = { 0 };
				theta.y = { 0 };
				theta.z = { 0 };

			}

			ImGui::End();

		}
	}


	virtual void setPos(float x, float y, float z);

	virtual void setRotation(float x, float y, float z);

	virtual void setDiminsion(float x, float y, float z);

	virtual void updatePos(float x, float y, float z);

	virtual void updateRotation(float x, float y, float z);

	virtual void updateDiminsion(float x, float y, float z);

public:

	ObjModel(Graphics& gfx, GCLASS& gclass)
		:Drawable(&gclass), gfx(gfx), pPCbuff(0), theta(0,0,0), scale(1.0,1.0,1.0),pos(0,0,0)
	{
	}

	ObjModel(Graphics& gfx):
		gfx(gfx), Drawable(nullptr), theta(0, 0, 0), scale(1.0, 1.0, 1.0), pos(0, 0, 0)
	{
	}

	~ObjModel() = default;
};


GCLASS* GenerateClassFromFile(Graphics& gfx, const char* pathToSrc);

