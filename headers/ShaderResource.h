#pragma once
#include "Bindable.h"

class ShaderResource :
	public Bindable 
{
private:
	ID3D11Buffer* pBuf;
	ID3D11ShaderResourceView* pView;

public:

	ShaderResource(Graphics& gfx, void* data,size_t size,BIND_STAGE bs, D3D11_USAGE GPUuse, D3D11_CPU_ACCESS_FLAG CPUuse):
        Bindable(SHADER_RESOURCE)
	{
        D3D11_SUBRESOURCE_DATA TmatData = { 0 };
        TmatData.pSysMem = data;

        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bd.Usage = GPUuse;
        bd.CPUAccessFlags = CPUuse;
        bd.MiscFlags = 0;
        bd.ByteWidth = size;
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, &TmatData, &pBuf));

	}


};

