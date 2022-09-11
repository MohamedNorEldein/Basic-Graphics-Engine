#pragma once
#include "Drawable.h"
#include "constantBuffer.h"
//#include "Graphics.h"


struct lightData {
	 __declspec(align(16)) DirectX::XMFLOAT3 lightDir;
	 __declspec(align(16)) DirectX::XMFLOAT3 lightColor ;
	 __declspec(align(16)) DirectX::XMFLOAT3 ambient ;
	 __declspec(align(16)) DirectX::XMFLOAT3 diffuseColor;
	float diffuseIntensity ;


	lightData() {
		lightDir = { 0,0,0 };
		lightColor = { 1.0,0.0,1.0 };
		ambient = { 0.15,0.15,0.15 };
		diffuseColor = { 1.0,1.0,1.0 };
		diffuseIntensity = 1.0;
}
};

class UniformLight 
{
private:
	lightData li;
	UINT SLOT;
	PixelConstantBuffer<lightData> lightCbuff;
public:
	UniformLight(Graphics& gfx, UINT SLOT)
		:  SLOT(SLOT), lightCbuff(gfx,SLOT),li()
	{
	}

	void updateDir(const DirectX::XMFLOAT3& lightDir) {
		li.lightDir = lightDir;
	}

	void updateColor(const DirectX::XMFLOAT3& lightColor) {
		li.lightColor = lightColor;
	}

	void bind(Graphics& gfx) {
		lightCbuff.update(gfx, li);
		lightCbuff.bind(gfx);
	}

};

