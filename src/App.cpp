//
// Created by m7mdn on 7/23/2022.
//
#include <time.h>
#include "App.h"


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
	//GirlModel.GuiControl();

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

	/* scene control */
	camera.CameraMouseControl(window.mouseEvent);
	camera.CameraKeyboardCotrol(window.keyBoardEvent);
	/* Imgui */



	/*window end frame stuff */
	gfx.EndFrame();
	return 0;
}

App::App() : window(0, 1200, 600), gfx(window)
, lamp(gfx, 0u)
//, GirlModel(gfx,"Models src data", "nanosuit.obj" )
,camera(gfx)
{

	gfx.setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5, 10000.0f));
	
	/*
	GirlModel.setPos(0, 0, 10);
	GirlModel.setDiminsion(10, 10, 10);
	GirlModel.setRotation(0, MATH_PI, 0);
	*/
	lamp.updateDir({ -1.0,1.0,-1.0 });

}

App::~App()
{

	return;
}


