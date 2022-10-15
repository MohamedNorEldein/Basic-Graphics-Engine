//
// Created by m7mdn on 7/23/2022.
//
#include <time.h>
#include "App.h"


std::vector<Node> vArray = {
	Node{	2.0f,0.0f,0.0f	,0	,-10,0	,-1	},		// 0
	Node{	1.0f,2.0f,0.0f	,0	,0	,0	,-1	},		// 1
	Node{	3.0f,2.0f,0.0f	,0	,0	,0	,-1	},		//	2
	Node{	0.0f,0.0f,0.0f	,0	,0	,0	,0	},
	Node{	4.0f,0.0f,0.0f	,0	,0	,0	,1	}
};

/*
std::vector<Node> vArray = {
	Node{	{2.0f,	0.0f,	0.0f},	{0	,-10,0}	,0	,{0	,0,0,0,0}, -1, 0},		// 0
	Node{	{1.0f,	2.0f,	0.0f},	{0	,0	,0}	,0	,{0	,0,0,0,0}, -1, 0},		// 1
	Node{	{3.0f,	2.0f,	0.0f},	{0	,0	,0}	,0	,{0	,0,0,0,0}, -1, 0},		//	2
	Node{	{0.0f,	0.0f,	0.0f},	{0	,0	,0}	,0	,{0	,0,0,0,0},	0, 0},
	Node{	{4.0f,	0.0f,	0.0f},	{0	,0	,0}	,0	,{0	,0,0,0,0},	1, 0}
};
*/
std::vector<Support> sArray =
{
Support{{0,1.0f,0},{1.0,0,0} },
Support{{ 0,1.0f,0 }},
};

std::vector<Member> mArray =
{
Member{0 ,1}, //0
Member{1 ,2}, //1
Member{2 ,4}, //2
Member{4 ,0}, //3
Member{3 ,0}, //4 
Member{1 ,3}, //5
Member{2 ,0}  //6
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
		
	system.Draw(gfx);
	/* scene control */
	camera.CameraMouseControl(window.mouseEvent);
	camera.CameraKeyboardCotrol(window.keyBoardEvent);
	
	
	/* Imgui */
	ModelsGuiControl(gfx);
	DrawAllModels(gfx);
	system.GUIcontrol();

	/*window end frame stuff */
	gfx.EndFrame();
	return 0;
}

App::App() : window(0, 1200, 600), gfx(*new Graphics(window))
, lamp(gfx, 0u)
//, GirlModel(gfx,"GraphicEngine\\Models src data", "nanosuit.obj" )
,camera()
,system(gfx)
{

	gfx.setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5, 10000.0f));
	gfx.setCamera(&camera);


	lamp.updateDir({ -1.0,1.0,-1.0 });
	/*
	system.push(vArray);
	system.push(mArray);
	system.push(sArray);
	*/

	system.push(L"CivilEngine\\shaders\\NodeShader.hlsl","node");
	system.push(L"CivilEngine\\shaders\\preMemberShader.hlsl","premember");
	system.push(L"CivilEngine\\shaders\\MemberShader.hlsl", "member");

	system.processData();

	system.bindMemberShader("member");
	system.bindNodeShader("node");
	system.bindPreMemberShader("premember");
	
}

App::~App()
{
	delete& gfx;
}


