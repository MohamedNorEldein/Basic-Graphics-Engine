#pragma once
#include "Bindable.h"
#include "ComputePipeLine.h"

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

	ShaderResource(ComputePipeLine& gfx, std::vector<float> &data, DXGI_FORMAT format, BIND_STAGE bs, UINT Slot, D3D11_USAGE GPUuse, D3D11_CPU_ACCESS_FLAG CPUuse) :
		Bindable(SHADER_RESOURCE), bs(bs), Slot(Slot)
	{
		D3D11_SUBRESOURCE_DATA TmatData = { 0 };
		TmatData.pSysMem = data.data();

		D3D11_BUFFER_DESC bd = { 0 };

		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.Usage = GPUuse;
		bd.CPUAccessFlags = CPUuse;
		bd.MiscFlags = 0;
		bd.ByteWidth = sizeof(float) * data.size();
		bd.StructureByteStride = 0;

		CHECK(gfx.getdevice()->CreateBuffer(&bd, &TmatData, &pBuf));
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};

		srvd.Format = format;
		srvd.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
		srvd.Buffer.ElementOffset = 0;
		srvd.Buffer.ElementWidth = 1;

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

	void bind(ComputePipeLine& gfx) {
		gfx.getcontext()->CSSetShaderResources(Slot, 1, &pView);
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

template<typename T>
class ComputeShaderInput :
	public Bindable
{
private:
	ID3D11Buffer* pBuf;
	ID3D11ShaderResourceView* pView;
	UINT Slot;

public:
	ComputeShaderInput(ComputePipeLine& gfx, std::vector<T>& data, UINT Slot) :
		Bindable(SHADER_RESOURCE), Slot(Slot)
	{
		D3D11_SUBRESOURCE_DATA TmatData = { 0 };
		TmatData.pSysMem = data.data();

		D3D11_BUFFER_DESC bd = { 0 };
		// DXGI_FORMAT_R32_FLOAT,0u, D3D11_BIND_UNORDERED_ACCESS, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE

		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE ;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.ByteWidth = sizeof(T) * data.size();
		bd.StructureByteStride = sizeof(T);

		CHECK(gfx.getdevice()->CreateBuffer(&bd, &TmatData, &pBuf));
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};

		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
		srvd.BufferEx.FirstElement = 0;
		srvd.BufferEx.Flags = 0;
		srvd.BufferEx.NumElements = data.size();

		CHECK(gfx.getdevice()->CreateShaderResourceView(pBuf, &srvd, &pView));
	}

	
	ComputeShaderInput(Graphics& gfx, std::vector<T>& data, UINT Slot) :
		Bindable(SHADER_RESOURCE), Slot(Slot)
	{
		D3D11_SUBRESOURCE_DATA TmatData = { 0 };
		TmatData.pSysMem = data.data();

		D3D11_BUFFER_DESC bd = { 0 };
		// DXGI_FORMAT_R32_FLOAT,0u, D3D11_BIND_UNORDERED_ACCESS, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE

		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.ByteWidth = sizeof(T) * data.size();
		bd.StructureByteStride = sizeof(T);

		CHECK(gfx.getdevice()->CreateBuffer(&bd, &TmatData, &pBuf));
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};

		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
		srvd.BufferEx.FirstElement = 0;
		srvd.BufferEx.Flags = 0;
		srvd.BufferEx.NumElements = data.size();

		(gfx.getdevice()->CreateShaderResourceView(pBuf, &srvd, &pView));


	}


	void bind(Graphics& gfx) {
		gfx.getcontext()->CSSetShaderResources(Slot, 1, &pView);
	}


	void bind(ComputePipeLine& gfx) {
		gfx.getcontext()->CSSetShaderResources(Slot, 1, &pView);
	}

	T* Map(Graphics& gfx) {

		D3D11_MAPPED_SUBRESOURCE TData;
		GetContext(gfx)->Map(pBuf, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &TData);
		return (T*)TData.pData;
	}

	T* Map(ComputePipeLine& gfx) {

		D3D11_MAPPED_SUBRESOURCE TData;
		gfx.getcontext()->Map(pBuf, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &TData);
		return (T*)TData.pData;
	}

	void Unmap(Graphics& gfx) {
		GetContext(gfx)->Unmap(pBuf, 0u);
	}

};

class ComputeShaderOutput :
	public Bindable
{
private:
	ID3D11Buffer* pBuf, *presult;
	ID3D11UnorderedAccessView* pView;
	UINT Slot;
	size_t size;
public:

	ComputeShaderOutput(PipeLine& gfx, UINT count, size_t item_size , UINT Slot) :
		Bindable(SHADER_RESOURCE), Slot(Slot), size(item_size* count)
	{
		/* create buffer to be used by gpu as output */
		
		D3D11_BUFFER_DESC bd = { 0 };
		bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.ByteWidth = size;
		bd.StructureByteStride = item_size;

		CHECK(gfx.getdevice()->CreateBuffer(&bd, nullptr, &pBuf));
		
		/* create buffer for gpu to write and cpu to read */
		bd.BindFlags = 0;
		bd.Usage = D3D11_USAGE_STAGING;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		
		CHECK(gfx.getdevice()->CreateBuffer(&bd, nullptr, &presult));

		/* unordered acsses view */


		D3D11_UNORDERED_ACCESS_VIEW_DESC uav = {};
		uav.Format = DXGI_FORMAT_UNKNOWN;
		uav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav.Buffer.FirstElement = 0;
		uav.Buffer.Flags = 0;
		uav.Buffer.NumElements = count;

		CHECK(gfx.getdevice()->CreateUnorderedAccessView(pBuf, &uav, &pView));

	}


	ComputeShaderOutput(PipeLine& gfx,void* data, UINT count, size_t item_size, UINT Slot) :
		Bindable(SHADER_RESOURCE), Slot(Slot), size(item_size* count)
	{
		/* create buffer to be used by gpu as output */
		D3D11_SUBRESOURCE_DATA Tdata = { 0 };
		Tdata.pSysMem = data;

		/* create buffer to be used by gpu as output */

		D3D11_BUFFER_DESC bd = { 0 };
		bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.ByteWidth = size;
		bd.StructureByteStride = item_size;

		CHECK(gfx.getdevice()->CreateBuffer(&bd, &Tdata, &pBuf));

		/* create buffer for gpu to write and cpu to read */
		bd.BindFlags = 0;
		bd.Usage = D3D11_USAGE_STAGING;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		CHECK(gfx.getdevice()->CreateBuffer(&bd, nullptr, &presult));

		/* unordered acsses view */


		D3D11_UNORDERED_ACCESS_VIEW_DESC uav = {};
		uav.Format = DXGI_FORMAT_UNKNOWN;
		uav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav.Buffer.FirstElement = 0;
		uav.Buffer.Flags = 0;
		uav.Buffer.NumElements = count;

		CHECK(gfx.getdevice()->CreateUnorderedAccessView(pBuf, &uav, &pView));

	}

	template<typename T>
	ComputeShaderOutput(PipeLine& gfx,std::vector<T> data, UINT Slot) :
		Bindable(SHADER_RESOURCE), Slot(Slot),size(sizeof(T)* data.size())
	{
		/* create buffer to be used by gpu as output */
		D3D11_SUBRESOURCE_DATA Tdata = { 0 };
		Tdata.pSysMem = data.data();

		D3D11_BUFFER_DESC bd = { 0 };
		bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.ByteWidth = sizeof(T) * data.size();
		bd.StructureByteStride = sizeof(T);

		CHECK(gfx.getdevice()->CreateBuffer(&bd, &Tdata, &pBuf));

		/* create buffer for gpu to write and cpu to read */
		bd.BindFlags = 0;
		bd.Usage = D3D11_USAGE_STAGING;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ| D3D11_CPU_ACCESS_WRITE;

		CHECK(gfx.getdevice()->CreateBuffer(&bd, nullptr, &presult));

		/* unordered acsses view */


		D3D11_UNORDERED_ACCESS_VIEW_DESC uav = {};
		uav.Format = DXGI_FORMAT_UNKNOWN;
		uav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav.Buffer.FirstElement = 0;
		uav.Buffer.Flags = 0;
		uav.Buffer.NumElements = data.size();

		CHECK(gfx.getdevice()->CreateUnorderedAccessView(pBuf, &uav, &pView));

	}
	
	
	~ComputeShaderOutput() {
		pBuf->Release();
		pView->Release();
	}

	void bind(Graphics& gfx) {
		gfx.getcontext()->CSSetUnorderedAccessViews(Slot, 1, &pView,0);
	}


	void bind(ComputePipeLine& gfx) {
		gfx.getcontext()->CSSetUnorderedAccessViews(Slot, 1, &pView,0);
	}

	template<typename T>
	T* read(ComputePipeLine& gfx) {
		gfx.getcontext()->CopyResource(presult,pBuf);
		D3D11_MAPPED_SUBRESOURCE TData;
		CHECK(gfx.getcontext()->Map(presult, 0u, D3D11_MAP_READ, 0u, &TData));
		gfx.getcontext()->Unmap(presult, 0u);
		return (T*)TData.pData;
	}

	template<typename T>
	T* read(Graphics& gfx) {
		gfx.getcontext()->CopyResource(presult, pBuf);
		D3D11_MAPPED_SUBRESOURCE TData;
		CHECK(gfx.getcontext()->Map(presult, 0u, D3D11_MAP_READ, 0u, &TData));
		gfx.getcontext()->Unmap(presult, 0u);
		return (T*)TData.pData;
	}

	void write(Graphics& gfx, void* data) {
		gfx.getcontext()->CopyResource(presult, pBuf);
		D3D11_MAPPED_SUBRESOURCE TData;
		CHECK(gfx.getcontext()->Map(presult, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &TData));
		memcpy(TData.pData, data, size);
		gfx.getcontext()->Unmap(presult, 0u);

	}

	void write(ComputePipeLine& gfx, void* data) {
		gfx.getcontext()->CopyResource(presult, pBuf);
		D3D11_MAPPED_SUBRESOURCE TData;
		CHECK(gfx.getcontext()->Map(presult, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &TData));
		memcpy(TData.pData, data, size);
		gfx.getcontext()->Unmap(presult, 0u);

	}


};
