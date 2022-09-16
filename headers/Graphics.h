//
// Created by m7mdn on 7/24/2022.
//

#include <mndWindow.h>
#include <d3d11.h>
#include <mndErrors.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// imgui
#include "imgui_impl_dx11.h"

#include "imgui_impl_win32.h"


#ifndef WINDOW_CPP_GRAPHICS_H
#define WINDOW_CPP_GRAPHICS_H

class Graphics{
private:
    ID3D11Device* pdevice;  
    // allocating memmory on d3d side
    IDXGISwapChain* pswapChain;
    // the front and back buffer swap
    ID3D11DeviceContext* pcontext;
    // for rendering
    ID3D11RenderTargetView* pTarget;
    // for view

    ID3D11DepthStencilView* pdsv;
private:

    DirectX::XMMATRIX projection;
    DirectX::XMFLOAT3 CameraPosition, cameraRotation;

    int width, height;

public:
    Graphics(HWND windowHandel, int width, int height);
    Graphics(const Graphics&) = delete;
    Graphics& operator = (const Graphics&) = delete;

    ~Graphics();

public:
    void BegainFrame();
    void EndFrame();
    void clearBuffer(float red, float green, float blue);
    void setViewPort(int x, int y, int width, int height);

public:
    inline void setProjection(const DirectX::XMMATRIX& mat) {
        projection = mat;
    }

    void updateCameraPosition(float x, float y);

    void updatePosition(float z);

    void updateScale(float z);

    void updateCameraRotation(float x, float y);

    void updateCameraRotation(int x, int y);

    DirectX::XMMATRIX getProjection();

    DirectX::XMMATRIX getCamera();


    void CreatetestTriangle();

public:
    inline IDXGISwapChain* getswapChain() {
        return pswapChain;
    }
    inline ID3D11DeviceContext* getcontext() {
        return pcontext;
    }
    inline ID3D11RenderTargetView* getTarget() {
        return pTarget;
    }
    inline ID3D11Device* getdevice() {
        return pdevice;
    }


private:

    friend class Bindable;
};

#endif //WINDOW_CPP_GRAPHICS_H
