//
// Created by m7mdn on 7/24/2022.
//

#include "mndWindow.h"


#include "Events.h"


#include <d3d11.h>
#include <mndErrors.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// imgui
#include "imgui_impl_dx11.h"

#include "imgui_impl_win32.h"


#define CameraScorollingSpeed 10.0f
#define CameraRotationSpeed		MATH_PI / (200.0f)

#ifndef WINDOW_CPP_GRAPHICS_H
#define WINDOW_CPP_GRAPHICS_H

int printf(const DirectX::XMVECTOR& v);
int printf(const DirectX::XMMATRIX& mat);


class FirstPearsonPerspective
{
protected:
    DirectX::XMFLOAT3 CameraPosition, cameraRotation;
    static float cx, cy, cz, rx, ry, rz;
    float CameraTransilationSpeed;
public:

    FirstPearsonPerspective();

    virtual ~FirstPearsonPerspective() = default;

    DirectX::XMMATRIX getCameraProjection();

    virtual void updateCameraPosition(float x, float y, float z);

    virtual void updateCameraRotation(float x, float y,float z);

    void CameraMouseControl(MouseEvents& mouseEvent);

    void CameraKeyboardCotrol(KeyBoardEvent keyBoardEvent);

    void GUIcontrol();

};

class ThirdPearsonPerspective :
    public FirstPearsonPerspective {
    
public:
    ThirdPearsonPerspective();
    void updateCameraPosition(float x, float y, float z);
    void updateCameraRotation(float _departure, float _lattude, float r);
    ~ThirdPearsonPerspective() = default;
};

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
    ThirdPearsonPerspective camera;
    DirectX::XMMATRIX projection;
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
    DirectX::XMMATRIX& getProjection();
    DirectX::XMMATRIX getCameraProjection();
    FirstPearsonPerspective& getCamera();

public:

    void setProjection(const DirectX::XMMATRIX& mat);
    IDXGISwapChain* getswapChain();
    ID3D11DeviceContext* getcontext();
    ID3D11RenderTargetView* getTarget();
    ID3D11Device* getdevice();
private:

    friend class Bindable;
};

#endif //WINDOW_CPP_GRAPHICS_H
