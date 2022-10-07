#pragma once


#include <d3d11.h>
#include <mndErrors.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>


int printf(const DirectX::XMVECTOR& v);
int printf(const DirectX::XMMATRIX& mat);



class ComputePipeLine
{
private:
    ID3D11Device* pdevice;
    // allocating memmory on d3d side

    ID3D11DeviceContext* pcontext;
    // for rendering
    ID3D11RenderTargetView* pTarget;
    // for view

    ID3D11DepthStencilView* pdsv;
private:


public:
    ComputePipeLine();

   
    ~ComputePipeLine();

public:

    ID3D11DeviceContext* getcontext();
    ID3D11Device* getdevice();

private:

    friend class Bindable;
};

