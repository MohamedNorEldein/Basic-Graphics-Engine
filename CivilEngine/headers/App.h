//
// Created by m7mdn on 7/23/2022.
//

#include "WindowClass.h"
#include "Light.h"
#include "ObjModel.h"
#include "cube.h"
#include "StaticSystem.h"


#ifndef MAIN_CPP_APP_H
#define MAIN_CPP_APP_H

class App {
private:
    // Basic D3D stuff
    Window window;
    Graphics gfx;

private:
    // models
   
    Structure1D system;
   // ObjModel GirlModel;// , HouseModel;
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
