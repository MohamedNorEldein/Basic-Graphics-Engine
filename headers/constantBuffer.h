
#include "Bindable.h"

#ifndef WINDOW_CPP_BINDABLE_H
#define WINDOW_CPP_BINDABLE_H


class DynamicConstantBuffer :
    public Bindable
{
protected:
    ID3D11Buffer* pconstantBuffer;
    UINT Slot;

public:

    DynamicConstantBuffer(Graphics& gfx , void* cb, unsigned int Slot, size_t size, bindableType BT = _unspecified):
        Slot(Slot),Bindable(BT)
    {
        D3D11_SUBRESOURCE_DATA TmatData = { 0 };
        TmatData.pSysMem = cb;

        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DYNAMIC;

        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        bd.ByteWidth = size;
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, &TmatData, &pconstantBuffer));
    }

  
    DynamicConstantBuffer(Graphics& gfx, UINT Slot , size_t size, bindableType BT = _unspecified):
    Slot(Slot), Bindable(BT)
    {
        
        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DYNAMIC;

        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        bd.ByteWidth = size;
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, 0, &pconstantBuffer));
    }


    ~DynamicConstantBuffer() override{
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
    public DynamicConstantBuffer
{
public:

    template <class T>
    PixelConstantBuffer(Graphics& gfx,const T& cb, UINT Slot ): DynamicConstantBuffer( gfx,(void*)& cb, Slot, sizeof(T),_PixelConstantBuffer)
    {
    }

    PixelConstantBuffer(Graphics& gfx, UINT Slot, size_t size) :DynamicConstantBuffer(gfx,Slot, size, _PixelConstantBuffer)
    {
       
    }

    void bind(Graphics& gfx) override
    {
        Bindable::GetContext(gfx)->PSSetConstantBuffers(Slot, 1, &(pconstantBuffer));
    }

};


class VertexConstantBuffer :
    public DynamicConstantBuffer
{

public:
    template <class T>
    VertexConstantBuffer(Graphics& gfx,const T& cb, UINT Slot) : DynamicConstantBuffer( gfx, &cb, Slot, sizeof(T))
    {
    }

    VertexConstantBuffer(Graphics& gfx, UINT Slot, size_t size) : DynamicConstantBuffer(gfx,Slot, size)
    {
    }

    void bind(Graphics& gfx) override
    {
        Bindable::GetContext(gfx)->VSSetConstantBuffers(Slot, 1, &(pconstantBuffer));
    }

};


template <class T>
class StaticConstantBuffer :
    public Bindable
{
protected:
    ID3D11Buffer* pconstantBuffer;
    UINT Slot;

public:
    StaticConstantBuffer(Graphics& gfx, T& cb, UINT Slot) :
        Slot(Slot), Bindable(_unspecified)
    {
        D3D11_SUBRESOURCE_DATA TmatData = { 0 };
        TmatData.pSysMem = &cb;

        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DEFAULT;

        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;

        bd.ByteWidth = sizeof(T);
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, &TmatData, &pconstantBuffer));
    }

    StaticConstantBuffer(Graphics& gfx, UINT Slot):
        Slot(Slot), Bindable(_unspecified)
    {

        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DEFAULT;

        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;

        bd.ByteWidth = sizeof(T);
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, 0, &pconstantBuffer));
    }


    ~StaticConstantBuffer() override {
        //cout("delete ConstantBuffer");
        pconstantBuffer->Release();
    }

};




template <class T>
class PixelStaticConstantBuffer :
    public StaticConstantBuffer<T>
{
public:

    PixelStaticConstantBuffer(Graphics& gfx, T& cb, UINT Slot) : StaticConstantBuffer<T>::StaticConstantBuffer(gfx, cb, Slot)
    {

    }

    PixelStaticConstantBuffer(Graphics& gfx, UINT Slot) : StaticConstantBuffer<T>::StaticConstantBuffer(gfx, Slot)
    {

    }

    void bind(Graphics& gfx) override
    {
        Bindable::GetContext(gfx)->PSSetConstantBuffers(StaticConstantBuffer<T>::Slot, 1, &(StaticConstantBuffer<T>::pconstantBuffer));
    }

};


template <class T>
class VertexStaticConstantBuffer :
    public StaticConstantBuffer<T>
{

public:
    VertexStaticConstantBuffer(Graphics& gfx, T& cb,UINT Slot) : StaticConstantBuffer<T>::StaticConstantBuffer(gfx, cb, Slot)
    {
    }

    VertexStaticConstantBuffer(Graphics& gfx, UINT Slot) : StaticConstantBuffer<T>::StaticConstantBuffer(gfx, Slot)
    {
    }
    void bind(Graphics& gfx) override
    {

        Bindable::GetContext(gfx)->VSSetConstantBuffers(StaticConstantBuffer<T>::Slot, 1, &(StaticConstantBuffer<T>::pconstantBuffer));
    }

};



#endif 
