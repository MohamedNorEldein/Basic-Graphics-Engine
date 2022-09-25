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


//GCLASS* GenerateCubeGCLASS(Graphics& );

class Cube :
	public Drawable
{
public:

	__declspec(align(16))
		struct COLOR {
		float r, b, g;
	};

	struct vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 Normal;
	};

private:
	DirectX::XMFLOAT3 pos, theta, scale;
	COLOR c;
	static GCLASS* CUBE_GCLASS;
	Graphics& gfx;
	TransformCBuffer* tr;

public:
	static PixelConstantBuffer* pcb;

public:
	Cube(Graphics& gfx);

	void Draw();

	void setPos(float x, float y, float z) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
	}

	void setRotation(float x, float y, float z) {
		theta.x = x;
		theta.y = y;
		theta.z = z;
	}

	void setDiminsion(float x, float y, float z) {
		scale.x = x;
		scale.y = y;
		scale.z = z;
	}

	void updatePos(float x, float y, float z) {
		pos.x += x;
		pos.y += y;
		pos.z += z;
	}

	void updateRotation(float x, float y, float z) {
		theta.x += x;
		theta.y += y;
		theta.z += z;
	}

	void updateDiminsion(float x, float y, float z) {
		scale.x = x;
		scale.y = y;
		scale.z = z;
	}

	void GuiControl() {
		/*	change parameters of the currently selected node */
		ImGui::Text("Position");
		ImGui::SliderFloat("position x", &pos.x, -5000.0f, +5000.0f);
		ImGui::SliderFloat("position y", &pos.y, -5000.0f, +5000.0f);
		ImGui::SliderFloat("position z", &pos.z, 0.0f, +5000.0f);

		ImGui::Text("Rotation");
		ImGui::SliderAngle("rotation x", &theta.x, -180.0f, +180.0f);
		ImGui::SliderAngle("rotation y", &theta.y, -180.0f, +180.0f);
		ImGui::SliderAngle("rotation z", &theta.z, -180.0f, +180.0f);

		ImGui::Text("Scale");
		ImGui::SliderFloat("scale x", &scale.x, 0.0f, +1000.0f);
		ImGui::SliderFloat("scale y", &scale.y, 0.0f, +1000.0f);
		ImGui::SliderFloat("scale z", &scale.z, 0.0f, +1000.0f);


		if (ImGui::Button("reset", ImVec2(50, 25))) {
			theta.x = { 0 };
			theta.y = { 0 };
			theta.z = { 0 };
			pos.x = { 0 };
			pos.y = { 0 };
			pos.z = { 0 };

		}
	}
};

