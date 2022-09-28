
#include "Bindable.h"

#ifndef WINDOW_CPP_BINDABLE_H
#define WINDOW_CPP_BINDABLE_H


class DynamicInputCBuffer :
    public Bindable
{
protected:
    ID3D11Buffer* pconstantBuffer;
    UINT Slot;

public:

    DynamicInputCBuffer(Graphics& gfx , void* cb, unsigned int Slot, size_t size, bindableType BT = _unspecified):
        Slot(Slot),Bindable(BT)
    {
        D3D11_SUBRESOURCE_DATA TmatData = { 0 };
        TmatData.pSysMem = cb;

        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DYNAMIC;

        bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        bd.ByteWidth = size;
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, &TmatData, &pconstantBuffer));
    }

  
    DynamicInputCBuffer(Graphics& gfx, UINT Slot , size_t size, bindableType BT = _unspecified ):
    Slot(Slot), Bindable(BT)
    {
        
        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DYNAMIC;

        bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        bd.ByteWidth = size;
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, 0, &pconstantBuffer));
    }


    ~DynamicInputCBuffer() override{
        //cout("delete ConstantBuffer");
        pconstantBuffer->Release();
    }

    
    void update(Graphics& gfx, void *subData, size_t size) {

        D3D11_MAPPED_SUBRESOURCE TData;
        GetContext(gfx)->Map( pconstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &TData);
        memcpy(TData.pData, subData, size);
         
        GetContext(gfx)->Unmap(pconstantBuffer, 0u);
    }

    template <class T>
    void update(Graphics& gfx, const T& subData) {

        D3D11_MAPPED_SUBRESOURCE TData;
        GetContext(gfx)->Map(pconstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &TData);
        memcpy(TData.pData, &subData, sizeof(T));

        GetContext(gfx)->Unmap(pconstantBuffer, 0u);
    }

};


class PixelConstantBuffer :
    public DynamicInputCBuffer
{
public:

    template <class T>
    PixelConstantBuffer(Graphics& gfx,const T& cb, UINT Slot ): DynamicInputCBuffer( gfx,(void*)& cb, Slot, sizeof(T),_PixelConstantBuffer)
    {
    }

    PixelConstantBuffer(Graphics& gfx, UINT Slot, size_t size) :DynamicInputCBuffer(gfx,Slot, size, _PixelConstantBuffer)
    {
       
    }

    void bind(Graphics& gfx) override
    {
        Bindable::GetContext(gfx)->PSSetConstantBuffers(Slot, 1, &(pconstantBuffer));
    }

};


class VertexConstantBuffer :
    public DynamicInputCBuffer
{

public:
    template <class T>
    VertexConstantBuffer(Graphics& gfx,const T& cb, UINT Slot) : DynamicInputCBuffer( gfx, &cb, Slot, sizeof(T))
    {
    }

    VertexConstantBuffer(Graphics& gfx, UINT Slot, size_t size) : DynamicInputCBuffer(gfx,Slot, size)
    {
    }

    void bind(Graphics& gfx) override
    {
        Bindable::GetContext(gfx)->VSSetConstantBuffers(Slot, 1, &(pconstantBuffer));
    }

};


class ComputeInputCBuffer :
    public DynamicInputCBuffer
{

public:
    template <class T>
    ComputeInputCBuffer(Graphics& gfx, const T& cb, UINT Slot) : DynamicInputCBuffer(gfx, &cb, Slot, sizeof(T))
    {
    }

    ComputeInputCBuffer(Graphics& gfx, UINT Slot, size_t size) : DynamicInputCBuffer(gfx, Slot, size)
    {
    }

    void bind(Graphics& gfx) override
    {
        Bindable::GetContext(gfx)->CSSetConstantBuffers(Slot, 1, &(pconstantBuffer));
    }

};


class DynamicOutputCBuffer :
    public Bindable
{
protected:
    ID3D11Buffer* pconstantBuffer;
    UINT Slot;

public:

    DynamicOutputCBuffer(Graphics& gfx, void* cb, unsigned int Slot, size_t size, bindableType BT = _unspecified) :
        Slot(Slot), Bindable(BT)
    {
        D3D11_SUBRESOURCE_DATA TmatData = { 0 };
        TmatData.pSysMem = cb;

        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DYNAMIC;

        bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        bd.MiscFlags = 0;

        bd.ByteWidth = size;
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, &TmatData, &pconstantBuffer));
    }


    DynamicOutputCBuffer(Graphics& gfx, UINT Slot, size_t size, bindableType BT = _unspecified) :
        Slot(Slot), Bindable(BT)
    {

        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = 0;
        bd.Usage = D3D11_USAGE_STAGING;

        bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        bd.MiscFlags = 0;

        bd.ByteWidth = size;
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, 0, &pconstantBuffer));
    }


    ~DynamicOutputCBuffer() override {
        //cout("delete ConstantBuffer");
        pconstantBuffer->Release();
    }


    void update(Graphics& gfx, void* subData, size_t size) {

        D3D11_MAPPED_SUBRESOURCE TData;
        GetContext(gfx)->Map(pconstantBuffer, 0u, D3D11_MAP_READ, 0u, &TData);
        memcpy(TData.pData, subData, size);

        GetContext(gfx)->Unmap(pconstantBuffer, 0u);
    }

    template <class T>
    void update(Graphics& gfx, const T& subData) {

        D3D11_MAPPED_SUBRESOURCE TData;
        GetContext(gfx)->Map(pconstantBuffer, 0u, D3D11_MAP_READ, 0u, &TData);
        memcpy(TData.pData, &subData, sizeof(T));

        GetContext(gfx)->Unmap(pconstantBuffer, 0u);
    }

};


class ComputeOutputCBuffer :
    public DynamicOutputCBuffer
{

public:
    template <class T>
    ComputeOutputCBuffer(Graphics& gfx, const T& cb, UINT Slot) : DynamicOutputCBuffer(gfx, &cb, Slot, sizeof(T))
    {
    }

    ComputeOutputCBuffer(Graphics& gfx, UINT Slot, size_t size) : DynamicOutputCBuffer(gfx, Slot, size)
    {
    }

    void bind(Graphics& gfx) override
    {
        Bindable::GetContext(gfx)->CSSetConstantBuffers(Slot, 1, &(pconstantBuffer));
    }

};





#endif 
