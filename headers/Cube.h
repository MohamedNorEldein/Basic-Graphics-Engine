#pragma once
#include "Gobject.h"



class Cube :
    public Gobject<Cube>
{
private:
   
    struct pixelCbuffData {
        color c[6];
        vertex normal[6];
    };

  
private:
    // data 
    // vertex buffer data
    std::vector<vertex> vBuffData = {
    { -1.0f, -1.0f, -1.0f},
    {  1.0f, -1.0f, -1.0f},
    { -1.0f,  1.0f, -1.0f},
    {  1.0f,  1.0f, -1.0f },

    { -1.0f, -1.0f,  1.0f},
    {  1.0f, -1.0f,  1.0f},
    { -1.0f,  1.0f,  1.0f},
    {  1.0f,  1.0f,  1.0f},
    };

    // index buffer data
    std::vector<unsigned short> indeces =
        {
        
        0,2,1,	2,3,1, //bottom
        1,3,5,	3,7,5, // front
        2,6,3,	3,6,7, //right
        4,5,7,	4,7,6, // top
        0,4,2,	2,4,6, //back
        0,1,4,	1,5,4  //left
    };

    std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    pixelCbuffData colorArray =
    {
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0,0,1,
        1,1,0,
        0,1,1,
        1,0,1,
    

         0.0f,-1.0f, 0.0f, // Bottom Side
         0.0f, -1.0f, 0.0f, // Bottom Side

        0.0f, 0.0f, 1.0f, // front Side
         0.0f, 0.0f, 1.0f, // front Side

           1.0f, 0.0f, 0.0f, // right Side
         1.0f, 0.0f, 0.0f, // right Side

         0.0f, 1.0f, 0.0f, // top Side
         0.0f, 1.0f, 0.0f, // top Side

         0.0f, 0.0f, -1.0f, // Back Side
         0.0f, 0.0f, -1.0f, // Back Side

        -1.0f, 0.0f, 0.0f, // Left Side
        -1.0f, 0.0f, 0.0f // Left Side

    };
   

private:
    void Init() {
        
        VertexShader* vs = new VertexShader(gfx, L"VertexShader1.cso");
        AddBindable(new IndexBuffer(gfx,indeces),_Indexbuffer);
        AddBindable(new VertexBuffer(gfx, vBuffData));

        AddBindable(vs);
        AddBindable(new PixelShader(gfx, L"PixelShader1.cso"));

        AddBindable(new InputLayout(gfx, ied,vs->getpBlob()));

        AddBindable(new PixelConstantBuffer(gfx, colorArray,0));
        AddBindable(new TransformCBuffer(gfx),_TransforCBuffer);
        setPrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

public:
    Cube(Graphics& gfx) :
        Gobject<Cube>(gfx)
    {
        if (isNotInitialized()) {
            Init();
        }
    }

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
};

