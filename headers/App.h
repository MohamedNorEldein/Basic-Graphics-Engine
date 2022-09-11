//
// Created by m7mdn on 7/23/2022.
//

#include "WindowClass.h"
#include "Light.h"
#include "AssModel.h"


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
    Window window;
    //Plane plane1;
  //  Prism<4> prism, prism1;
    AssModel model1, model2, model3;
    UniformLight sun;

    float v[2] = { 0 }, u[2] = { 0 };
    int d1 = 0;


public:
    App();
    ~App();
    WPARAM go();

private:
    WPARAM doFrame();

    void updateView();
    void mouseHandeler();

};


#endif //MAIN_CPP_APP_H
