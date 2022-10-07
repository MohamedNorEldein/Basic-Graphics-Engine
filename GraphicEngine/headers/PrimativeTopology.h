#pragma once
#include "Bindable.h"


class PrimativeTopology :
    public Bindable
{
private:
    D3D11_PRIMITIVE_TOPOLOGY pt;

public:
    PrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY pt):
        pt(pt), Bindable(BINDABLE_TYPE::TOPOLOGY)
    {
    }

    void bind(Graphics& gfx) override {
       GetContext(gfx)->IASetPrimitiveTopology(pt);
    }

    ~PrimativeTopology() = default;
};

