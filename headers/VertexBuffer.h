#pragma once
#include "Bindable.h"
#include "InputLayout.h"





class VertexBuffer :
    public Bindable
{
private:
	const UINT stride;
	const UINT offset;
	ID3D11Buffer* pVertexBuffer;

public:
    template<typename T>
    VertexBuffer(Graphics& gfx,std::vector<T> vertecies) :stride(sizeof(T)), offset(0u), Bindable(BINDABLE_TYPE::VERTEX_BUFFER)

	{
		printf("%d\n", sizeof(T));
		D3D11_SUBRESOURCE_DATA vdata = { 0 };
		vdata.pSysMem = vertecies.data();

		D3D11_BUFFER_DESC BufferDesc = { 0 };

		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		BufferDesc.CPUAccessFlags = 0u;
		BufferDesc.MiscFlags = 0u;

		BufferDesc.StructureByteStride = sizeof(T);
		BufferDesc.ByteWidth = vertecies.size() * sizeof(T);

		CHECK(GetDevice(gfx)->CreateBuffer(&BufferDesc, &vdata, &pVertexBuffer));
    }

	template<typename T>
	VertexBuffer(Graphics& gfx, T* vertecies, int count) :stride(sizeof(T)), offset(0u), Bindable(BINDABLE_TYPE::VERTEX_BUFFER)

	{
		cout(vertecies);
		D3D11_SUBRESOURCE_DATA vdata = { 0 };
		vdata.pSysMem = vertecies;

		D3D11_BUFFER_DESC BufferDesc = { 0 };

		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		BufferDesc.CPUAccessFlags = 0u;
		BufferDesc.MiscFlags = 0u;

		BufferDesc.StructureByteStride = sizeof(T);
		BufferDesc.ByteWidth = count * sizeof(T);

		CHECK(GetDevice(gfx)->CreateBuffer(&BufferDesc, &vdata, &pVertexBuffer));
	}

	VertexBuffer(Graphics& gfx, void* vertecies, int count, size_t sizeofVertex) :stride(sizeofVertex), offset(0u), Bindable(BINDABLE_TYPE::VERTEX_BUFFER)

	{
		cout(vertecies);
		D3D11_SUBRESOURCE_DATA vdata = { 0 };
		vdata.pSysMem = vertecies;

		D3D11_BUFFER_DESC BufferDesc = { 0 };

		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		BufferDesc.CPUAccessFlags = 0u;
		BufferDesc.MiscFlags = 0u;

		BufferDesc.StructureByteStride = sizeofVertex;
		BufferDesc.ByteWidth = count * sizeofVertex;

		CHECK(GetDevice(gfx)->CreateBuffer(&BufferDesc, &vdata, &pVertexBuffer));
	}

	VertexBuffer(Graphics& gfx, vertexBufferData& vbd) : stride(vbd.getSizeOfVertex()), offset(0u), Bindable(BINDABLE_TYPE::VERTEX_BUFFER)
	{
		D3D11_SUBRESOURCE_DATA vdata = { 0 };
		vdata.pSysMem = vbd.Data();

		D3D11_BUFFER_DESC BufferDesc = { 0 };

		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		BufferDesc.CPUAccessFlags = 0u;
		BufferDesc.MiscFlags = 0u;

		BufferDesc.StructureByteStride = vbd.getSizeOfVertex();
		BufferDesc.ByteWidth = vbd.getCount() * vbd.getSizeOfVertex();

		CHECK(GetDevice(gfx)->CreateBuffer(&BufferDesc, &vdata, &pVertexBuffer));
		
	}

	void bind(Graphics& gfx) override{
		GetContext(gfx)->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);
		

	}

	~VertexBuffer() {
		cout("delete vertex buffer");
		pVertexBuffer->Release();
	}

};

