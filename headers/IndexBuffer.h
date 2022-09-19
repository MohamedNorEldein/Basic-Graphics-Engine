#pragma once
#include "Bindable.h"

class IndexBuffer:public Bindable
{
private:
	ID3D11Buffer* pindexBuffer = 0;
	unsigned int count;

public:
	IndexBuffer(Graphics& gfx, std::vector<unsigned short> indecies) 
		:count(indecies.size()), Bindable(_Indexbuffer)

	{
		D3D11_SUBRESOURCE_DATA idata = { 0 };
		D3D11_BUFFER_DESC indexBufferDesc = { 0 };

		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		indexBufferDesc.ByteWidth = sizeof(unsigned short) * indecies.size();
		indexBufferDesc.StructureByteStride = sizeof(unsigned short);

		indexBufferDesc.CPUAccessFlags = 0u;
		indexBufferDesc.MiscFlags = 0u;

		idata.pSysMem = indecies.data();

		GetDevice(gfx)->CreateBuffer(&indexBufferDesc, &idata, &pindexBuffer);
	}

	IndexBuffer(Graphics& gfx, unsigned short* indecies, int count)
		:count(count), Bindable(_Indexbuffer)

	{
		D3D11_SUBRESOURCE_DATA idata = { 0 };
		D3D11_BUFFER_DESC indexBufferDesc = { 0 };

		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		indexBufferDesc.ByteWidth = sizeof(unsigned short) * count;
		indexBufferDesc.StructureByteStride = sizeof(unsigned short);

		indexBufferDesc.CPUAccessFlags = 0u;
		indexBufferDesc.MiscFlags = 0u;

		idata.pSysMem = indecies;

		GetDevice(gfx)->CreateBuffer(&indexBufferDesc, &idata, &pindexBuffer);
	}

	~IndexBuffer() {
		pindexBuffer->Release();
	}

	void bind(Graphics& gfx)
	{
		GetContext(gfx)->IASetIndexBuffer(pindexBuffer, DXGI_FORMAT_R16_UINT, 0);
		

	}

	unsigned int getIndecesNumber() {
		return count;
	}


};

