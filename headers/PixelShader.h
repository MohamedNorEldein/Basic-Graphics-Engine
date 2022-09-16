#pragma once
#include "Bindable.h"
#include <cstring>

class PixelShader :
    public Bindable
{
private:
    ID3D11PixelShader* pPixelShaders =  0;
    ID3DBlob* pBlob;
    //wchar_t psName[250];

public:
    PixelShader(Graphics& gfx, const wchar_t* pixelShaderCompiledFileName = L"PixelShader1.cso"):  Bindable(_PixelShader)
    {

        CHECK(D3DReadFileToBlob(pixelShaderCompiledFileName, &pBlob));
        CHECK(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), 0, &pPixelShaders));

    }

    ~PixelShader() {
        pPixelShaders->Release();
    }

    void bind(Graphics& gfx) override {
        GetContext(gfx)->PSSetShader(pPixelShaders, 0, 0);

    }

    ID3DBlob* getpBlob() {
        return pBlob;
    }

};

