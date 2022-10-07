#pragma once
#include "Drawable.h"

#include "constantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "CBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "IndexBuffer.h"

template<class T>
class Gobject :
    public Drawable
{
protected:
    static std::vector<Bindable*> bindables;
    static IndexBuffer* pindexBuff;
    static TransformCBuffer* ptransformCbuff;
    static D3D11_PRIMITIVE_TOPOLOGY PT;
    DirectX::XMFLOAT3 theta, scale, pos;
    Graphics& gfx;


    struct vertex {
        float x, y, z;
        float nx, ny, nz;
    };

    struct pixelStaticCbuffData {
        vertex normal[6];

    };

public:
    __declspec(align(16))

    struct color {
        float r, b, g;
    };
public:


    Gobject(Graphics& gfx)
        :gfx(gfx), theta(0, 0, 0), scale(1, 1, 1), pos(0, 0, 0)
    {
    }

    ~Gobject() = default;


    bool isNotInitialized() {
        return bindables.empty();
    }

    void setPrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY pt) override {
        PT = pt;
    }

    void AddBindable(Bindable* bindable, bindableType BT = _unspecified) override {
        bindables.push_back(bindable);

        switch (BT)
        {
        case bindableType::_unspecified:
            return;

        case bindableType::_Indexbuffer:
            pindexBuff = (IndexBuffer*)bindable;
          //  std::cout << bindable << '\n';
            return;
        case bindableType::_TransforCBuffer:
            ptransformCbuff = (TransformCBuffer*)bindable;
            return;
        default:
            return;
        }

    }


    virtual void Draw(Graphics& gfx) {
        ptransformCbuff->setDrawableParent(this);

        for (auto b : bindables) {
            b->bind(gfx);
        }
        pindexBuff->GetContext(gfx)->IASetPrimitiveTopology(PT);
        pindexBuff->GetContext(gfx)->DrawIndexed(pindexBuff->getIndecesNumber(), 0, 0);
    }

    virtual DirectX::XMMATRIX GetTransform()  {
        using namespace DirectX;
        return 
            XMMatrixScalingFromVector(XMLoadFloat3(&scale))
            *
            XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&theta))
            *
            XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
    }


    virtual void setPos(float x, float y, float z) {
        pos.x = x;
        pos.y = y;
        pos.z = z;
    }

    virtual void setRotation(float x, float y, float z) {
        theta.x = x;
        theta.y = y;
        theta.z = z;
    }

    virtual void setDiminsion(float x, float y, float z) {
        scale.x = x;
        scale.y = y;
        scale.z = z;
    }

    virtual void updatePos(float x, float y, float z) {
        pos.x += x;
        pos.y += y;
        pos.z += z;
    }

    virtual void updateRotation(float x, float y, float z) {
        theta.x += x;
        theta.y += y;
        theta.z += z;
    }

    virtual void updateDiminsion(float x, float y, float z) {
        scale.x = x;
        scale.y = y;
        scale.z = z;
    }


};

template<class T>
std::vector<Bindable*> Gobject<T>::bindables;

template<class T>
IndexBuffer* Gobject<T>::pindexBuff;

template<class T>
D3D11_PRIMITIVE_TOPOLOGY Gobject<T>::PT;

template<class T>
TransformCBuffer* Gobject<T>::ptransformCbuff;




