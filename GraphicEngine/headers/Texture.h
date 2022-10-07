#pragma once

#include "Bindable.h"
#include "ImageFactory.h"
#include "WICTextur.h"

/*
* the Gpu have VRAM which is like RAM contains data , any block of memory on VRAM is called Resource
* Resources could be Buffers or Texture
*   Textures:
*       Texture1D
*       Texture2D
*       Texture3D
* the texture could be used in many places throw the Graphic pipeline.
*   ShaderResource :
*       is the texture used by the shaders at diffrant stages
*   ShaderResourceView:
*        is the view of the shader in the memory
*
*/

#define MAX_SLOT_COUNT 5

class Texture :
	public Bindable
{
private:
	ID3D11ShaderResourceView* pTextureView;
	Image* img;
	ID3D11Texture2D* pTexture;
	UINT slot;

public:
	Texture(Graphics& gfx, const wchar_t* src, UINT slot = 0u)
		:Bindable(BINDABLE_TYPE::TEXTURE), pTexture(nullptr), pTextureView(nullptr), slot(slot)
	{

		img = new Image(src, gfx.getdevice(), 0u);

		// Create texture
		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = img->getWidth();
		desc.Height = img->getHeight();
		desc.MipLevels = 1u;
		desc.ArraySize = 1u;
		desc.Format = img->getFormat();
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;


		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;


		D3D11_SUBRESOURCE_DATA initData = { 0 };

		initData.pSysMem = img->getData();
		initData.SysMemPitch = img->getRowPitch();
		initData.SysMemSlicePitch = img->getImageSize();

		gfx.getdevice()->CreateTexture2D(&desc, &initData, &pTexture);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;

		srvd.Format = img->getFormat();
		srvd.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1u;
		srvd.Texture2D.MostDetailedMip = 0u;

		CHECK(gfx.getdevice()->CreateShaderResourceView(pTexture, &srvd, &pTextureView));

		delete img;

	}

	Texture(Graphics& gfx, Image& img, UINT slot = 0u)
		:Bindable(BINDABLE_TYPE::TEXTURE), pTexture(nullptr), pTextureView(nullptr), slot(slot)
	{
		// Create texture
		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = img.getWidth();
		desc.Height = img.getHeight();
		desc.MipLevels = 1u;
		desc.ArraySize = 1u;
		desc.Format = img.getFormat();
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = { 0 };

		initData.pSysMem = img.getData();
		initData.SysMemPitch = img.getRowPitch();
		initData.SysMemSlicePitch = img.getImageSize();

		gfx.getdevice()->CreateTexture2D(&desc, &initData, &pTexture);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;

		srvd.Format = img.getFormat();
		srvd.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1u;
		srvd.Texture2D.MostDetailedMip = 0u;

		CHECK(gfx.getdevice()->CreateShaderResourceView(pTexture, &srvd, &pTextureView));

	}

	void bind(Graphics& gfx) override {
		gfx.getcontext()->PSSetShaderResources(slot, 1u, &pTextureView);
	}

	~Texture() {
		pTextureView->Release();
		pTexture->Release();
	}

};
