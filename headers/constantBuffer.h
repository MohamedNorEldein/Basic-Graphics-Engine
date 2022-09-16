
#include "Drawable.h"

#ifndef WINDOW_CPP_BINDABLE_H
#define WINDOW_CPP_BINDABLE_H

template <class T>
class DynamicConstantBuffer :
    public Bindable
{
protected:
    ID3D11Buffer* pconstantBuffer;
    UINT Slot;

public:
    DynamicConstantBuffer(Graphics& gfx , T& cb, unsigned int Slot ):
        Slot(Slot),Bindable(_unspecified)
    {
        D3D11_SUBRESOURCE_DATA TmatData = { 0 };
        TmatData.pSysMem = &cb;

        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DYNAMIC;

        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        bd.ByteWidth = sizeof(T);
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, &TmatData, &pconstantBuffer));
    }

    DynamicConstantBuffer(Graphics& gfx, unsigned int Slot ):
    Slot(Slot), Bindable(_unspecified)
    {
        
        D3D11_BUFFER_DESC bd = { 0 };

        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.Usage = D3D11_USAGE_DYNAMIC;

        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        bd.ByteWidth = sizeof(T);
        bd.StructureByteStride = 0;

        CHECK(GetDevice(gfx)->CreateBuffer(&bd, 0, &pconstantBuffer));
    }


    ~DynamicConstantBuffer() override{
        //cout("delete ConstantBuffer");
        pconstantBuffer->Release();
    }

    void update(Graphics& gfx, const T& subData) {

        D3D11_MAPPED_SUBRESOURCE TData;
        GetContext(gfx)->Map( pconstantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &TData);
        memcpy(TData.pData, &subData, sizeof(subData));
         
        GetContext(gfx)->Unmap(pconstantBuffer, 0u);
    }

};

template <class T>
class PixelConstantBuffer :
    public DynamicConstantBuffer<T>
{
public:

    PixelConstantBuffer(Graphics& gfx, T& cb, UINT Slot ): DynamicConstantBuffer<T>::DynamicConstantBuffer( gfx, cb,Slot)
    {

    }

    PixelConstantBuffer(Graphics& gfx, UINT Slot) : DynamicConstantBuffer<T>::DynamicConstantBuffer(gfx,Slot)
    {

    }

    void bind(Graphics& gfx) override
    {
        Bindable::GetContext(gfx)->PSSetConstantBuffers(DynamicConstantBuffer<T>::Slot, 1, &(DynamicConstantBuffer<T>::pconstantBuffer));
    }

};


template <class T>
class VertexConstantBuffer :
    public DynamicConstantBuffer<T>
{

public:
    VertexConstantBuffer(Graphics& gfx, T& cb, UINT Slot) : DynamicConstantBuffer<T>::DynamicConstantBuffer( gfx, cb, Slot)
    {
    }

    VertexConstantBuffer(Graphics& gfx, UINT Slot) : DynamicConstantBuffer<T>::DynamicConstantBuffer(gfx,Slot)
    {
    }
    void bind(Graphics& gfx) override
    {

        Bindable::GetContext(gfx)->VSSetConstantBuffers(DynamicConstantBuffer<T>::Slot, 1, &(DynamicConstantBuffer<T>::pconstantBuffer));
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
