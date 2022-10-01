#pragma once
#include "Drawable.h"
#include "constantBuffer.h"
//#include "Graphics.h"


struct lightData {
	 __declspec(align(16)) DirectX::XMFLOAT3 lightDir;
	 __declspec(align(16)) DirectX::XMFLOAT3 lightColor ;
	 __declspec(align(16)) DirectX::XMFLOAT3 ambient ;
	float diffuseIntensity, ambientIntensity;


	lightData() {
		lightDir = { 1.0,0,0 };
		lightColor = { 1.0,1.0,1.0 };
		ambient = { 1.0,1.0,1.0 };
		diffuseIntensity = 0.1050;
		ambientIntensity = 0.4;
}
};

class UniformLight 
{
private:
	lightData li;
	UINT SLOT;
	PixelConstantBuffer lightCbuff;
public:
	UniformLight(Graphics& gfx, UINT SLOT)
		:  SLOT(SLOT), lightCbuff(gfx,SLOT, sizeof(lightData)),li()
	{
	}

	void updateDir(const DirectX::XMFLOAT3& lightDir) {
		li.lightDir = lightDir;
	}

	void updateColor(const DirectX::XMFLOAT3& lightColor) {
		li.lightColor = lightColor;
	}

	void updateAmbientColor(const DirectX::XMFLOAT3& lightColor) {
		 DirectX::XMStoreFloat3(&(li.lightColor), DirectX::XMVector3Normalize(DirectX::XMLoadFloat3( &lightColor)));
	}

	void bind(Graphics& gfx) {
		lightCbuff.update(gfx, li);
		lightCbuff.bind(gfx);
	}

	void GuiControl() {
		if (ImGui::Begin("light", nullptr, (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))) {
			ImGui::SliderFloat3("position", (float*)&li.lightDir, -1.0, +1.0);
			ImGui::SliderFloat3("ambient", (float*)&li.ambient, 0, +1.0);
			ImGui::SliderFloat3("diffuse color", (float*)&li.lightColor, 0.0, +1.0);
			ImGui::SliderFloat("Diffuse Intenisty", (float*)&li.diffuseIntensity, 0, 1.0f);
			ImGui::SliderFloat("Ampient Intenisty", (float*)&li.ambientIntensity, 0, 1.0f);

		}
		ImGui::End();
	}

};


struct PointlightData {
	 __declspec(align(16)) DirectX::XMFLOAT3 lightPos;
	 __declspec(align(16)) DirectX::XMFLOAT3 ambient;
	 __declspec(align(16)) DirectX::XMFLOAT3 diffuseColor;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;

	PointlightData() {
		lightPos = { 0,0,0 };
		ambient = { 0.15,0.15,0.15 };
		diffuseColor = { 1.0,1.0,1.0 };
		diffuseIntensity = 0.5;
		attConst = 0.1;
		attLin = 0.2; 
		attQuad = 0.01;
	}
};


class PointSource
{
private:
	PointlightData li;
	UINT SLOT;
	PixelConstantBuffer lightCbuff;
public:
	PointSource(Graphics& gfx, UINT SLOT)
		: SLOT(SLOT), lightCbuff(gfx, SLOT, sizeof(lightData)), li()
	{
	}

	void updatePos(const DirectX::XMFLOAT3& pos) {
		li.lightPos = pos;
	}

	void updateColor(const DirectX::XMFLOAT3& lightColor) {
		li.diffuseColor = lightColor;
	}

	void bind(Graphics& gfx) {
		lightCbuff.update(gfx, li);
		lightCbuff.bind(gfx);
	}

	void GuiControl() {

		if (ImGui::Begin("light")) {
			ImGui::SliderFloat3("position", (float*) & li.lightPos, -5000, +5000);
			ImGui::SliderFloat3("ambient", (float*)&li.ambient,0, +1.0);
			ImGui::SliderFloat3("diffuse color", (float*)&li.diffuseColor, 0.0, +1.0);


		}
		ImGui::End();
	}

};

