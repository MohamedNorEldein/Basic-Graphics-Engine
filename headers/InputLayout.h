#pragma once
#include "Bindable.h"


class InputLayout :
    public Bindable
{
private:
    ID3D11InputLayout* pInputLayout;
public:

    InputLayout(Graphics& gfx, std::vector<D3D11_INPUT_ELEMENT_DESC > layOuts, ID3DBlob* pvertexShaderBlob) {

        GetDevice(gfx)->CreateInputLayout(layOuts.data(), layOuts.size(), pvertexShaderBlob->GetBufferPointer(), pvertexShaderBlob->GetBufferSize(), &pInputLayout);
    }

    ~InputLayout() {
        pInputLayout->Release();
    }

    void bind(Graphics& gfx) {
        GetContext(gfx)->IASetInputLayout(pInputLayout);
    }

    
};

