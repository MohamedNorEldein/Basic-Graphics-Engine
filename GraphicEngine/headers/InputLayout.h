#pragma once
#include "Bindable.h"



class LayoutStrucure {

private:
	
	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	std::vector<size_t> sizeVec;
	UINT vertexSize;
public:
	
public:
	LayoutStrucure() :
		vertexSize(0u)
	{
	}

	LayoutStrucure(LayoutStrucure&) = delete;
	LayoutStrucure(LayoutStrucure&&) = delete;
	
	~LayoutStrucure() = default;

	UINT size() {
		return vertexSize;
	}

	UINT count() {
		return desc.size();
	}

	int operator[](const char* semantic) {
		int i = 0;
		for (auto& a : desc) {
			if (!strcmp(semantic, a.SemanticName)) {
				return i;
			}
			i++;
		}

		throw std::exception();
	}

	constexpr D3D11_INPUT_ELEMENT_DESC* data() {
		return desc.data();
	}

	friend class Item;

public:
	template<typename T>
	void Append(const char* semantics) {};

	template<>
	void Append<DirectX::XMFLOAT4>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, vertexSize, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		
		UINT newSize = 4 * sizeof(float);
		sizeVec.push_back(newSize);
		vertexSize += newSize;
	}

	template<>
	void Append<DirectX::XMFLOAT3>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, vertexSize, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		
		UINT newSize = 3 * sizeof(float);
		sizeVec.push_back(newSize);

		vertexSize += newSize;
	}

	template<>
	void Append<DirectX::XMFLOAT2>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, vertexSize, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize = 2 * sizeof(float);
		sizeVec.push_back(newSize);

		vertexSize += newSize;
	}

	template<>
	void Append<float>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, 0, vertexSize, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize =  sizeof(float);
		sizeVec.push_back(newSize);

		vertexSize += newSize;
	}

	template<>
	void Append<int>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32_SINT, 0, vertexSize, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize = sizeof(int);
		sizeVec.push_back(newSize);

		vertexSize += newSize;
	}

	template<>
	void Append<UINT>(const char* semantics) {
		desc.push_back({ semantics, 0, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0, vertexSize, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize = sizeof(int);
		sizeVec.push_back(newSize);

		vertexSize += newSize;
	}

	template<>
	void Append<DirectX::XMINT2>(const char* semantics) {
		desc.push_back({ semantics,0, DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT, 0, vertexSize, D3D11_INPUT_PER_VERTEX_DATA, 0});

		UINT newSize = 2 * sizeof(int);
		sizeVec.push_back(newSize);

		vertexSize += newSize;
	}

	template<>
	void Append<DirectX::XMINT3>(const char* semantics) {
		desc.push_back({ semantics,0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT, 0, vertexSize, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		UINT newSize = 3 * sizeof(int);
		sizeVec.push_back(newSize);

		vertexSize += newSize;
	}

	friend class BufferData;
};


class InputLayout :
	public Bindable
	
{
private:
	ID3D11InputLayout* pInputLayout;
public:

	InputLayout(Graphics& gfx, std::vector<D3D11_INPUT_ELEMENT_DESC >& layOuts, ID3DBlob* pvertexShaderBlob) :
		Bindable(BINDABLE_TYPE::LAYOUT)
	{

		GetDevice(gfx)->CreateInputLayout(layOuts.data(), layOuts.size(), pvertexShaderBlob->GetBufferPointer(), pvertexShaderBlob->GetBufferSize(), &pInputLayout);
	}

	InputLayout(Graphics& gfx,const D3D11_INPUT_ELEMENT_DESC* layOuts,UINT count, ID3DBlob* pvertexShaderBlob) :
		Bindable(BINDABLE_TYPE::LAYOUT)
	{

		GetDevice(gfx)->CreateInputLayout(layOuts, count, pvertexShaderBlob->GetBufferPointer(), pvertexShaderBlob->GetBufferSize(), &pInputLayout);
	}

	InputLayout(Graphics& gfx, LayoutStrucure& layOuts, ID3DBlob* pvertexShaderBlob) :
		Bindable(BINDABLE_TYPE::LAYOUT)
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

class Item {
private:
	byte* data;
	LayoutStrucure& layout;
public:
	Item(byte* data, LayoutStrucure& layout):
		data(data), layout(layout)
	{
	}

	template<typename T>
	T Data (UINT i) {
		return *(T*)(data + layout.desc[i].AlignedByteOffset);
	}

	template<typename T>
	T& operator[](UINT i) {
		return *(T*)(data + layout.desc[i].AlignedByteOffset);
	}


};

class BufferData {
private:
	LayoutStrucure& layout;
	byte* data;
	size_t sizeOfVertex, count;

public:
	BufferData(LayoutStrucure& layout,size_t count) :
		layout(layout), data((byte*)malloc(layout.size() * count )), sizeOfVertex(layout.size()) , count(count)
	{
		printf("vertex size : %d\n", sizeOfVertex);
	}

	void addData(const char* semantic, void* dataArray) {		 
		 int a = layout[semantic];
		 UINT i = 0u, stride= layout.desc[a].AlignedByteOffset, size = layout.sizeVec[a];
		 byte* _d = data + stride, * _s = (byte*)dataArray;

		 while( i < count) {
			 memcpy(_d , _s, size);

			 _d += sizeOfVertex;
			 _s += size;
			 i++;
		 }

	}

	Item operator[](UINT vertexIndex) {
		return Item(data + vertexIndex * layout.vertexSize,layout);
	}

	D3D11_INPUT_ELEMENT_DESC* getLayout() {
		return layout.desc.data();
	}

	void print() {
		size_t stride = 0;
		
		for (auto& l : layout.desc) {
			printf("\n[%s] = < %u  >\n",
				l.SemanticName,
				l.AlignedByteOffset);

			stride += 3 * sizeof(float);
		}

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

	~BufferData() {
		delete data;
	}

};