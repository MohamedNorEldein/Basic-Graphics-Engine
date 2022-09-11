#pragma once
#include "Gobject.h"



template<int FaceNum = 0>
class Prism :
    public Drawable
{

protected:
    static std::vector<Bindable*> bindables;
    static IndexBuffer* pindexBuff;
    static TransformCBuffer* ptransformCbuff;
    static D3D11_PRIMITIVE_TOPOLOGY PT;
    DirectX::XMFLOAT3 theta, scale, pos;
    Graphics& gfx;

public:
    

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
            return;
        case bindableType::_TransforCBuffer:
            ptransformCbuff = (TransformCBuffer*)bindable;
            return;
        default:
            return;
        }

    }

    void _Draw(Graphics& gfx) {
        ptransformCbuff->setDrawableParent(this);

        for (auto b : bindables) {
            b->bind(gfx);
        }
        pindexBuff->GetContext(gfx)->IASetPrimitiveTopology(PT);
        pindexBuff->GetContext(gfx)->DrawIndexed(pindexBuff->getIndecesNumber(), 0, 0);
    }

public:
    void Draw(Graphics& gfx) override {
        pccb->update(gfx, clr);
        _Draw(gfx);
    }


    virtual DirectX::XMMATRIX GetTransform() {
        using namespace DirectX;
        return
            XMMatrixScalingFromVector(XMLoadFloat3(&scale))

            *
            XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&theta))
            
            *
            XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
    }




private:
    // data types
    struct Vertex {
        float x, y, z;
    };

public:
    struct color {
        float r, b, g, a;
    };

private:
    // data 
    
    PixelConstantBuffer<color>* pccb = 0;

    // vertex buffer data
    Vertex vBuffData[2*FaceNum];

    // index buffer data
    unsigned short indeces[FaceNum*6 + (FaceNum-2) *6 ] = {0};

    std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    color clr;

private:

    void generate() {

        float x, y;
        int j, numOfPoints = FaceNum * 2;

        
        for (int i = 0; i < FaceNum ; ++i) {
            x = 0.5 * sin(MATH_PI * 2 * i / FaceNum);
            y = 0.5 * cos(MATH_PI * 2 * i / FaceNum);

            vBuffData[2 * i] = { x,y,0.5 };
            vBuffData[2 * i + 1] = { x,y,-0.5 };
        }
        
        for (int i = 0; i < FaceNum; ++i) {
            j = 6 * i;
            indeces[j] = (2 * i);
            indeces[j + 2] = (2 * i + 1) % numOfPoints;
            indeces[j + 1] = (2 * i + 3) % numOfPoints;

            indeces[j + 3] = (2*i);
            indeces[j + 5] = (2*i + 3)%numOfPoints;
            indeces[j + 4] = (2*i + 2) % numOfPoints;

        }
        
        j = FaceNum * 6;
        for (int i = 1; i < FaceNum -1; ++i) {
            indeces[j ] = 0;
            indeces[j+1] = (2 * i + 2)%numOfPoints;
            indeces[j+2] = (2 * i) % numOfPoints;
            j += 3;
        }
        
        j = FaceNum * 6 + (FaceNum - 2) * 3;
        for (int i = 1; i < FaceNum - 1; ++i) {
            indeces[j ] = 1;
            indeces[j+1] = (2 * i + 1) % numOfPoints;
            indeces[j+2] = (2 * i + 3) % numOfPoints;
            j += 3;
        }
    }

    void Init() {
        generate();

        VertexShader* vs = new VertexShader(gfx);
        AddBindable(new IndexBuffer(gfx, indeces, FaceNum * 6 + (FaceNum - 2) * 6), _Indexbuffer);
        AddBindable(new VertexBuffer(gfx, vBuffData, 2 * FaceNum));

        AddBindable(vs);
        AddBindable(new PixelShader(gfx,L"PrismPixelShader.cso"));

        AddBindable(new InputLayout(gfx, ied, vs->getpBlob()));
        
        AddBindable(new TransformCBuffer(gfx), _TransforCBuffer);

        setPrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

public:
   
    Prism(Graphics& gfx) :
        clr({ 0,0,0,0 }) , gfx(gfx), theta(0, 0, 0), scale(1, 1, 1), pos(0, 0, 0)

    {
        if (isNotInitialized()) {
            Init();
        }
        pccb = new PixelConstantBuffer<Prism::color>(gfx);
        AddBindable(pccb);

    }

    ~Prism() {
        delete pccb;
    };
public:

    void setPos(float x, float y, float z) {
        pos.x = x;
        pos.y = y;
        pos.z = z;
    }

    void setRotation(float x, float y, float z) {
        theta.x = x;
        theta.y = y;
        theta.z = z;
    }

    void setDiminsion(float x, float y, float z) {
        scale.x = x;
        scale.y = y;
        scale.z = z;
    }

    void updatePos(float x, float y, float z) {
        pos.x += x;
        pos.y += y;
        pos.z += z;
    }

    void updateRotation(float x, float y, float z) {
        theta.x += x;
        theta.y += y;
        theta.z += z;
    }

    void updateDiminsion(float x, float y, float z) {
        scale.x = x;
        scale.y = y;
        scale.z = z;
    }

    void setColor(float r, float g, float b, float a) {
        clr = { r,g,b,a };
    }

    void setColor(color& c) {
        clr = c;
    }

    color& getColor() {
        return clr;
    }

    

};



template<int T>
std::vector<Bindable*> Prism<T>::bindables;

template<int T>
IndexBuffer* Prism<T>::pindexBuff;

template<int T>
D3D11_PRIMITIVE_TOPOLOGY Prism<T>::PT;

template<int T>
TransformCBuffer* Prism<T>::ptransformCbuff;

