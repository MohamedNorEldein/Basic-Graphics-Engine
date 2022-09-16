#pragma once
#include "ObjModel.h"



class Prism :
    public ObjModel
{
public:
   
    Prism(Graphics& gfx);
    ~Prism();
};



GCLASS* GenerateCubeGCLASS(Graphics& gfx, int FaceNum);