//
// Created by m7mdn on 7/23/2022.
//

#include "WindowClass.h"
#include "Light.h"
#include "ObjModel.h"
#include "cube.h"
#include "ComputeShader.h"

#ifndef MAIN_CPP_APP_H
#define MAIN_CPP_APP_H

enum mouseEvetType {
    NONE = 0,
    CameraMoveFowrwad,
    Scale,
    CameraRotaote,
    CameraMove

};


class App {
private:
    // Basic D3D stuff
    Window window;
    Graphics gfx;

private:
    // models
    Cube model1, model2, model3 , model4;
    ObjModel objModel;
private:
    // camera and light
    ThirdPearsonPerspective camera;
    UniformLight lamp;

public:
    App();
    ~App();
    WPARAM go();

private:
    WPARAM doFrame();

};


#endif //MAIN_CPP_APP_H
