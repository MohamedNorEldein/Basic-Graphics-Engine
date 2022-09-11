//
// Created by m7mdn on 7/23/2022.
//
#include <time.h>
#include "App.h"


WPARAM App::go() {

    MSG msg;
     do{
       PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
    //while (GetMessage(&msg, nullptr, 0, 0)) {

        doFrame();
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        //+++++++

    }
     while (msg.message != WM_QUIT);

   
    return msg.wParam;

}
WPARAM App::doFrame() {
    /* window begain frame stuff*/
    
    window.Gfx().clearBuffer(0.5, 0.5, 0.5);
    window.Gfx().BegainFrame();

    sun.bind(window.Gfx());
    mouseHandeler();
    model1.GuiControl();

    /*App stuff*/
    model1.Draw(window.Gfx());
    model2.Draw(window.Gfx());
    model3.Draw(window.Gfx());
     /*ImGui stuff*/
    updateView();
   

    /*window end frame stuff */
    window.Gfx().EndFrame();
    return 0;
}

App::App() : window( L"hello world",1200,600), sun(window.Gfx(),0u)
, model1(window.Gfx()), model2(model1), model3(model1)

{

   window.Gfx().setProjection(
        DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5, 10000.0f)
    );

    model1.setDiminsion(100, 100, 100);
    model1.setPos(-1000, 0, 450);
    model1.setColor(0.0,1.0,0,1);

    model2.setDiminsion(100, 100, 100);
    model2.setPos(1000, 0, 450);
    model2.setColor(0.0, 1.0, 0, 1);

    model3.setDiminsion(100, 100, 100);
    model3.setPos(0, 0, 450);
    model3.setColor(0.0, 1.0, 0, 1);

    sun.updateDir({ -10,-10,1.2 });
    sun.updateColor({ 1.0,0,0 });
}

void App::updateView()
{
    

    if (ImGui::Begin("Camera")) {
        ImGui::SliderFloat2("camera position:",v, -5000.0f, +5000.0f);
        ImGui::SliderFloat2("camera Rotation:", u, -5.0f, +5.0f);

        window.Gfx().updateCameraPosition(v[0], v[1]);
        window.Gfx().updateCameraRotation(u[0], u[1]);
        if (ImGui::Button("reset", ImVec2(50, 25))) {
            v[0] = {0};
            v[1] = { 0 };

            u[0] = { 0 };
            u[1] = { 0 };
        }
        
        ImGui::End();

    }

}


void App::mouseHandeler() {


    
}

App::~App()
{
    std::cout << "delete app\n";
    
   
    return;
}


