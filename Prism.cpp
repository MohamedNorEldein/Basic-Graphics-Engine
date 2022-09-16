
#include "Prism.h"


typedef  DirectX::XMFLOAT3 vertex;


D3D11_INPUT_ELEMENT_DESC  ied[] = {
    { "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


Prism::Prism(Graphics& gfx) :
         ObjModel(gfx)
    {
    }

Prism::~Prism() {
    };




GCLASS* GenerateCubeGCLASS(Graphics& gfx,int FaceNum) {

    float x, y;
    int j, numOfPoints = FaceNum * 2;


    vertex* vBuffData = new vertex[numOfPoints];
    unsigned short* indeces = new unsigned short[FaceNum];

    for (int i = 0; i < FaceNum; ++i) {
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

        indeces[j + 3] = (2 * i);
        indeces[j + 5] = (2 * i + 3) % numOfPoints;
        indeces[j + 4] = (2 * i + 2) % numOfPoints;

    }

    j = FaceNum * 6;
    for (int i = 1; i < FaceNum - 1; ++i) {
        indeces[j] = 0;
        indeces[j + 1] = (2 * i + 2) % numOfPoints;
        indeces[j + 2] = (2 * i) % numOfPoints;
        j += 3;
    }

    j = FaceNum * 6 + (FaceNum - 2) * 3;
    for (int i = 1; i < FaceNum - 1; ++i) {
        indeces[j] = 1;
        indeces[j + 1] = (2 * i + 1) % numOfPoints;
        indeces[j + 2] = (2 * i + 3) % numOfPoints;
        j += 3;
    }
    char str[50] = {0};
    sprintf(str, "Prism%d", FaceNum);
    GCLASS* pgclass = new GCLASS(str);

    VertexShader* vs = new VertexShader(gfx);
    pgclass->AddBindable(new IndexBuffer(gfx, indeces, FaceNum * 6 + (FaceNum - 2) * 6));
    pgclass->AddBindable(new VertexBuffer(gfx, vBuffData, 2 * FaceNum));

    pgclass->AddBindable(vs);
    pgclass->AddBindable(new PixelShader(gfx, L"AssPixelShader.cso"));

    pgclass->AddBindable(new InputLayout(gfx, ied,1u, vs->getpBlob()));

    pgclass->AddBindable(new TransformCBuffer(gfx));

    pgclass->AddBindable( new PrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    delete[] vBuffData;
    delete[] indeces;

    return pgclass;
}

