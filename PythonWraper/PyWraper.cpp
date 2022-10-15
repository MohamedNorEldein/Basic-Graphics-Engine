#define Export __declspec(dllexport)

#include "PyWraper.h"

std::vector<Node> nodeVec;
std::vector<Member> memberVec;



Export void spam_system()
{
    printf("Hello world\n");
}

Export Structure1D* GetStructuralSystem() {
   // return &(app->system);
    return 0;
}

Export void pushNode(float x, float y, float z, float fx, float fy, float fz) {
    nodeVec.push_back(Node(x, y, z ,fx,fy,fz));
}

Export void pushMember(uint s , uint e) {
    memberVec.push_back(Member( s, e));
}