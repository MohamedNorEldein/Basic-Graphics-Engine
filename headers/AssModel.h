#pragma once
#include "Gobject.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

//#pragma comment (lib, "assimp-vc143-mtd")




class AssModel :
	public Gobject<AssModel>
{
private:
	 TransformCBuffer* ptransformCbuff;

	PixelConstantBuffer<color>* pccb = 0;

private:

	static vertex* vbData;
	static unsigned short* IBData;
	static std::vector<D3D11_INPUT_ELEMENT_DESC> ied;

private:
	void Generate(const char* modelSrc);

	void Init(const char* modelSrc);

public:
	void setColor(float r, float g, float b, float a);

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

public:
	AssModel(Graphics& gfx);

};


