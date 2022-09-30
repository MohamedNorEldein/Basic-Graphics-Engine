//
// Created by m7mdn on 7/24/2022.
//
#include <WindowClass.h>
#include <d3d11.h>
#include <mndErrors.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
// imgui
#include "imgui_impl_dx11.h"

#include "imgui_impl_win32.h"


#define CameraScorollingSpeed 10.0f
#define CameraRotationSpeed		MATH_PI / (50.0f + cameraRotation.m128_f32[1])

#ifndef WINDOW_CPP_GRAPHICS_H
#define WINDOW_CPP_GRAPHICS_H

int printf(const DirectX::XMVECTOR& v);
int printf(const DirectX::XMMATRIX& mat);

class FirstPearsonPerspective;

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
    FirstPearsonPerspective* camera;
    DirectX::XMMATRIX projection;
    int width, height;

    void init(HWND windowHandel, int width, int height);

public:
    Graphics(HWND windowHandel, int width, int height);
    Graphics(Window& window);

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
    void setCamera(FirstPearsonPerspective* _camera);
    void setProjection(const DirectX::XMMATRIX& mat);

public:
    IDXGISwapChain* getswapChain();
    ID3D11DeviceContext* getcontext();
    ID3D11RenderTargetView* getTarget();
    ID3D11Device* getdevice();
    
private:

    friend class Bindable;
};


class FirstPearsonPerspective
{
protected:
    DirectX::XMVECTOR CameraPosition, cameraRotation;
    static float cx, cy, cz, rx, ry, rz;
    float CameraTransilationSpeed;
public:

    FirstPearsonPerspective(Graphics&);

    virtual ~FirstPearsonPerspective() = default;

    DirectX::XMMATRIX getCameraProjection();

    virtual void updateCameraPosition(float x, float y, float z);

    virtual void updateCameraRotation(float x, float y, float z);

    void CameraMouseControl(MouseEvents& mouseEvent);

    void CameraKeyboardCotrol(KeyBoardEvent keyBoardEvent);

    void GUIcontrol();

};

class ThirdPearsonPerspective :
    public FirstPearsonPerspective {

public:
    ThirdPearsonPerspective(Graphics& camera);
    void updateCameraPosition(float x, float y, float z);
    void updateCameraRotation(float _departure, float _lattude, float r);
    ~ThirdPearsonPerspective() = default;
};


#endif //WINDOW_CPP_GRAPHICS_H
