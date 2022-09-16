#pragma once
#include "Bindable.h"

class VertexShader :
    public Bindable
{
private:
    ID3DBlob* pBlob;
    ID3D11VertexShader* pvertexShaders = 0 ;

public:
    VertexShader(Graphics& gfx, const wchar_t* vertexShaderCompiledFileName = L"VertexShader1.cso"): Bindable(_VertexShader)
    {

        CHECK(D3DReadFileToBlob(vertexShaderCompiledFileName, &pBlob));
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

