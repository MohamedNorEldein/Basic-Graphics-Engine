
#include "Bindable.h"

#ifndef WINDOW_CPP_BINDABLE_H
#define WINDOW_CPP_BINDABLE_H



class CBuffer :
    public Bindable
{
protected:
    ID3D11Buffer* pconstantBuffer;
    UINT Slot;
    BIND_STAGE bs;
public:

    CBuffer(Graphics& gfx , void* cb, unsigned int Slot, size_t size, BIND_STAGE bs):
        Slot(Slot),Bindable(BINDABLE_TYPE::CONSTANT_BUFFER),bs(bs)
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

  
    CBuffer(Graphics& gfx, UINT Slot , size_t size, BIND_STAGE bs):
    Slot(Slot), Bindable(CONSTANT_BUFFER), bs(bs)
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


    ~CBuffer() override{
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

    void bind(Graphics& gfx) override
    {
     
        switch (bs)
        {
        case VERTEX_SHADER_STAGE:
            Bindable::GetContext(gfx)->VSSetConstantBuffers(Slot, 1, &(pconstantBuffer));
            break;
        case PIXEL_SHADER_STAGE:
            Bindable::GetContext(gfx)->PSSetConstantBuffers(Slot, 1, &(pconstantBuffer));
            break;
        case COMPUTE_SHADER_STAGE:
            Bindable::GetContext(gfx)->CSSetConstantBuffers(Slot, 1, &(pconstantBuffer));
            break;
        default:
            break;
        }
    }

};





#endif 
