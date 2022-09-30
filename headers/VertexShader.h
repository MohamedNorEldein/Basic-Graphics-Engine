#pragma once
#include "Bindable.h"

class VertexShader :
    public Bindable
{
private:
    ID3DBlob* pBlob;
    ID3D11VertexShader* pvertexShaders = 0 ;

public:
    VertexShader(Graphics& gfx, const wchar_t* vertexShaderFileName , bool compiled = true): Bindable(BINDABLE_TYPE::VERTEX_SHADER)
    {
        if (compiled) {
            CHECK(D3DReadFileToBlob(vertexShaderFileName, &pBlob));
        }
        else {

#ifdef _DEBUG
            CHECK(D3DCompileFromFile(vertexShaderFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG, 0, &pBlob, NULL));
#else
            CHECK(D3DCompileFromFile(vertexShaderFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &pBlob, NULL));
#endif // _DEBUG

        }
        CHECK(GetDevice(gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), 0, &pvertexShaders));

    }

    ~VertexShader() {
        pvertexShaders->Release();
    }

    void bind(Graphics& gfx) override {
        GetContext(gfx)->VSSetShader(pvertexShaders, 0, 0);

    }

    ID3DBlob* getpBlob() {
        return pBlob;
    }
};

