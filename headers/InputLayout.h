#pragma once
#include "Bindable.h"



class LayoutStrucure {

private:
	typedef struct element {
		size_t stride, size;
	}element;
	
	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	std::vector<element> descData;

	size_t vertexSize;
public:
	

public:
	LayoutStrucure() :
		vertexSize(0u)
	{
	}

	LayoutStrucure(LayoutStrucure&) = delete;
	LayoutStrucure(LayoutStrucure&&) = delete;
	
	~LayoutStrucure() = default;

	size_t size() {
		return vertexSize;
	}

	size_t count() {
		return desc.size();
	}

	element& operator[](const char* semantic) {
		int i = 0;
		for (auto& a : desc) {
			if (!strcmp(semantic, a.SemanticName)) {
				return descData[i];
			}
			i++;
		}

		throw std::exception();
	}

	D3D11_INPUT_ELEMENT_DESC* data() {
		return desc.data();
	}

public:
	template<typename T>
	void Append(const char* semantics) {};

	template<>
	void Append<DirectX::XMFLOAT4>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		
		UINT newSize = 4 * sizeof(float);
		descData.push_back({ vertexSize,newSize });
		vertexSize += newSize;
	}

	template<>
	void Append<DirectX::XMFLOAT3>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		
		UINT newSize = 3 * sizeof(float);
		descData.push_back({ vertexSize,newSize });
		vertexSize += newSize;
	}

	template<>
	void Append<DirectX::XMFLOAT2>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize = 2 * sizeof(float);
		descData.push_back({ vertexSize,newSize });
		vertexSize += newSize;
	}

	template<>
	void Append<float>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize =  sizeof(float);
		descData.push_back({ vertexSize,newSize });
		vertexSize += newSize;
	}

	template<>
	void Append<int>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32_SINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize = sizeof(int);
		descData.push_back({ vertexSize,newSize });
		vertexSize += newSize;
	}

	template<>
	void Append<UINT>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize = sizeof(int);
		descData.push_back({ vertexSize,newSize });
		vertexSize += newSize;
	}

	template<>
	void Append<DirectX::XMINT2>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize = 2 * sizeof(int);
		descData.push_back({ vertexSize,newSize });
		vertexSize += newSize;
	}

	template<>
	void Append<DirectX::XMINT3>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize = 3 * sizeof(int);
		descData.push_back({ vertexSize,newSize });
		vertexSize += newSize;
	}

	friend class vertexBufferData;
};


class InputLayout :
	public Bindable
	
{
private:
	ID3D11InputLayout* pInputLayout;
public:

	InputLayout(Graphics& gfx, std::vector<D3D11_INPUT_ELEMENT_DESC >& layOuts, ID3DBlob* pvertexShaderBlob) :
		Bindable(_Layout)
	{

		GetDevice(gfx)->CreateInputLayout(layOuts.data(), layOuts.size(), pvertexShaderBlob->GetBufferPointer(), pvertexShaderBlob->GetBufferSize(), &pInputLayout);
	}

	InputLayout(Graphics& gfx, D3D11_INPUT_ELEMENT_DESC* layOuts,UINT count, ID3DBlob* pvertexShaderBlob) :
		Bindable(_Layout)
	{

		GetDevice(gfx)->CreateInputLayout(layOuts, count, pvertexShaderBlob->GetBufferPointer(), pvertexShaderBlob->GetBufferSize(), &pInputLayout);
	}

	InputLayout(Graphics& gfx, LayoutStrucure& layOuts, ID3DBlob* pvertexShaderBlob) :
		Bindable(_Layout)
	{

		GetDevice(gfx)->CreateInputLayout(layOuts.data(), layOuts.count(), pvertexShaderBlob->GetBufferPointer(), pvertexShaderBlob->GetBufferSize(), &pInputLayout);
	}

	~InputLayout() {
		pInputLayout->Release();
	}

	void bind(Graphics& gfx) {
		GetContext(gfx)->IASetInputLayout(pInputLayout);
		

	}


};

class vertexBufferData {
private:
	LayoutStrucure& layout;
	byte* data;
	size_t sizeOfVertex, count;

public:
	vertexBufferData(LayoutStrucure& layout,size_t count) :
		layout(layout), data((byte*)malloc(layout.size() * count )), sizeOfVertex(layout.size()) , count(count)
	{
	}

	void addData(const char* semantic, void* dataArray) {		 
		 LayoutStrucure::element& a = layout[semantic];

		 UINT i = 0u, stride= a.stride, size = a.size;
		 byte* _d = data + stride, * _s = (byte*)dataArray;

		 while( i < count) {
			 memcpy(_d , _s, size);

			 _d += sizeOfVertex;
			 _s += size;
			 i++;
		 }

	}

	D3D11_INPUT_ELEMENT_DESC* getLayout() {
		return layout.desc.data();
	}

	void print() {
		size_t stride = 0;
		for (UINT i = 0u; (i < count)&&(i<50); ++i) {
			stride = 0;
			for (auto& l : layout.desc) {
				printf("[%s] = < %f , %f, %f >\t",
					l.SemanticName,
					*(float*)((byte*)data + i * sizeOfVertex + stride),
					*(float*)((byte*)data + i * sizeOfVertex + stride + sizeof(float) ),
					* (float*)((byte*)data + i * sizeOfVertex + stride + 2* sizeof(float))
					);

				stride += 3 * sizeof(float);
			}
			printf("\n");
		}
	}

	void* Data() {
		return data;
	}

	size_t getSizeOfVertex() {
		return sizeOfVertex;
	}

	size_t getCount() {
		return count;
	}

	~vertexBufferData() {
		delete data;
	}

};