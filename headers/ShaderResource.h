#pragma once
#include "Bindable.h"

class ShaderResource :
	public Bindable
{
private:
	ID3D11Buffer* pBuf;
	ID3D11ShaderResourceView* pView;
	BIND_STAGE bs;
	UINT Slot;
public:

	ShaderResource(Graphics& gfx, void* data, size_t size, DXGI_FORMAT format, BIND_STAGE bs, UINT Slot, D3D11_USAGE GPUuse, D3D11_CPU_ACCESS_FLAG CPUuse) :
		Bindable(SHADER_RESOURCE), bs(bs), Slot(Slot)
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

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;

		srvd.Format = format;
		srvd.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
		srvd.Texture2D.MipLevels = 1u;
		srvd.Texture2D.MostDetailedMip = 0u;

		CHECK(gfx.getdevice()->CreateShaderResourceView(pBuf, &srvd, &pView));

	}

	void bind(Graphics& gfx) {
		switch (bs)
		{
		case VERTEX_SHADER_STAGE:
			Bindable::GetContext(gfx)->VSSetConstantBuffers(Slot, 1, &(pBuf));
			break;
		case PIXEL_SHADER_STAGE:
			Bindable::GetContext(gfx)->PSSetConstantBuffers(Slot, 1, &(pBuf));
			break;
		case COMPUTE_SHADER_STAGE:
			Bindable::GetContext(gfx)->CSSetConstantBuffers(Slot, 1, &(pBuf));
			break;
		default:
			break;
		}
	}


	void* Map(Graphics& gfx) {

		D3D11_MAPPED_SUBRESOURCE TData;
		GetContext(gfx)->Map(pBuf, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &TData);
		return TData.pData;
	}

	void* Unmap(Graphics& gfx) {
		GetContext(gfx)->Unmap(pBuf, 0u);
	}

};

