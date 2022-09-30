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
    
    PixelShader(Graphics& gfx, const wchar_t* pixelShaderFileName = L"VertexShader1.cso", bool compiled = true) : Bindable(BINDABLE_TYPE::PIXEL_SHADER)
    {
        if (compiled) {
            CHECK(D3DReadFileToBlob(pixelShaderFileName, &pBlob));
        }
        else {

#ifdef _DEBUG
            CHECK(D3DCompileFromFile(pixelShaderFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG, 0, &pBlob, NULL));
#else
            CHECK(D3DCompileFromFile(pixelShaderFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, &pBlob, NULL));
#endif // _DEBUG
        }
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

