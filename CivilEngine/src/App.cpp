//
// Created by m7mdn on 7/23/2022.
//
#include <time.h>
#include "App.h"


std::vector<Node> vArray = {
	Node{	{2.0f,	0.0f,	0.0f},	{0	,-10,0}	,4	,{0	,3,4,6,0}},		// 0
	Node{	{1.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{0	,1,5,0,0}},		// 1
	Node{	{3.0f,	2.0f,	0.0f},	{0	,0	,0}	,3	,{2	,1,6,0,0}}
};

std::vector<Support> sArray =
{
Support{	{0.0f,	0.0f,	0.0f},	{0,0,0}	,2	,{5	,4,0,0,0}	,1	,{{0,1.0f,0},{1.0,0,0},{0,0,0}},	{5.0,1.0,0} },		// 3
Support{	{4.0f,	0.0f,	0.0f},	{0,0,0}	,2	,{3	,2,0,0,0}	,1	,{{0,1.0f,0},{0.0,0,0},{0,0,0}},	{5.0,0,0}	},		// 4
};

std::vector<Member> mArray =
{
{{-0.447214	, 0.894427 	,0}	,	0 ,0 ,1	}, //0
{{1.000000	, 0.000000 	,0}	,   0 ,1 ,2	}, //1
{{0.447214	, -0.894427	,0}	,   0 ,2 ,4	}, //2
{{-1.000000	, 0.000000 	,0}	,	0 ,4 ,0	}, //3
{{1.000000	, 0.000000 	,0}	,	0 ,3 ,0	}, //4 
{{-0.447214	, -0.894427	,0}	,   0 ,1 ,3	}, //5
{{-0.447214	, -0.894427	,0}	,	0 ,2 ,0	}  //6
};


WPARAM App::go() {

	MSG msg;
	int k;

	do
	{
		k = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
		doFrame();
		if(k)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	} while (msg.message != WM_QUIT);


	return msg.wParam;

}
COLOR c = { 1.0,0.50,0.20 };

WPARAM App::doFrame() {
	/* window begain frame stuff*/
	ImGui::SetNextWindowBgAlpha(0.7f);

	gfx.clearBuffer(0.5, 0.5, 0.5);
	gfx.BegainFrame();
	
	lamp.bind(gfx);

	ImGui::SetNextWindowBgAlpha(0.7f);
	camera.GUIcontrol();
	lamp.GuiControl();

	if (window.keyBoardEvent.isDown(VK_TAB) && window.keyBoardEvent.type == WM_KEYDOWN) {

		if (window.mouseEvent.IsRawInputEnable()) {
			window.mouseEvent.DisableRawMouse();
			printf("raw Mouse input disable\n");

		}
		else {
			window.mouseEvent.EnableRawMouse();
			printf("raw Mouse input enable\n");
		}
	}
	//GirlModel.Draw();
	
	system.Draw(gfx);
	/* scene control */
	camera.CameraMouseControl(window.mouseEvent);
	camera.CameraKeyboardCotrol(window.keyBoardEvent);
	/* Imgui */
	ModelsGuiControl(gfx);
	DrawAllModels(gfx);

	/*window end frame stuff */
	gfx.EndFrame();
	return 0;
}

App::App() : window(0, 1200, 600), gfx(window)
, lamp(gfx, 0u)
//, GirlModel(gfx,"GraphicEngine\\Models src data", "nanosuit.obj" )
,camera(gfx)
,system(gfx)
{

	gfx.setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5, 10000.0f));
	
	/*
	GirlModel.setPos(0, 0, 10);
	GirlModel.setDiminsion(10);
	GirlModel.setRotation(0, MATH_PI, 0);
	*/
	lamp.updateDir({ -1.0,1.0,-1.0 });

	system.push("nodes", &vArray);
	system.push("members", &mArray);
	system.push("supports", &sArray);

	system.push(L"CivilEngine\\shaders\\MemberShader.hlsl");
	system.push(L"CivilEngine\\shaders\\NodeShader.hlsl");
	system.push(L"CivilEngine\\shaders\\preMemberShader.hlsl");
	
	system.bindMembersData("members");
	system.bindNodesData("nodes","supports");

	system.bindMemberShader("CivilEngine\\shaders\\MemberShader.hlsl");
	system.bindNodeShader("CivilEngine\\shaders\\NodeShader.hlsl");
	system.bindPreMemberShader("CivilEngine\\shaders\\preMemberShader.hlsl");

}

App::~App()
{

	return;
}


