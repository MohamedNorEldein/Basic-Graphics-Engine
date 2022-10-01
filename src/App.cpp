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
	c = { 1,0,0 };
	
	//model1.pcb->update(gfx, c);
	model1.Draw();

	c = { 0,1.0,0 };
	//model2.pcb->update(gfx, c);
	model2.Draw();

	c = { 0,0,1 };
	//model3.pcb->update(gfx, c);
	model3.Draw();

	c = { 0,1,1 };
	//model4.pcb->update(gfx, c);
	model4.Draw();
	
	objModel.pcb->update(gfx, c);
	objModel.Draw();

	/* scene control */
	camera.CameraMouseControl(window.mouseEvent);
	camera.CameraKeyboardCotrol(window.keyBoardEvent);
	/* Imgui */
	ImGui::SetNextWindowBgAlpha(0.7f);
	camera.GUIcontrol();
	lamp.GuiControl();
	objModel.GuiControl();


	/*window end frame stuff */
	gfx.EndFrame();
	return 0;
}

App::App() : window(L"hello world", 1200, 600), gfx(window)
, lamp(gfx, 0u)
, objModel(gfx, "Models src data\\gvv1e7u547-Borderlands 2 - Maya\\Borderlands 2 - Maya\\maya.obj", _countof("Models src data\\gvv1e7u547 - Borderlands 2 - Maya\\Borderlands 2 - Maya\\"))
//, objModel(gfx, "Models src data\\Man_with_suit.obj", _countof("Models src data\\"))
, model1(gfx)
, model2(gfx)
, model3(gfx)
, model4(gfx)
,camera(gfx)
{

	gfx.setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5, 10000.0f));
	
	model1.setPos(-100, 0, 100);
	model1.setDiminsion(50, 50, 50);

	model2.setPos(100, 0, 100);
	model2.setDiminsion(50, 50, 50);

	model3.setPos(100, 0, -100);
	model3.setDiminsion(50, 50, 50);

	model4.setPos(-100, 0, -100);
	model4.setDiminsion(50, 50, 50);

	objModel.setPos(0, 0, 100);
	objModel.setDiminsion(10,10,10);

	lamp.updateDir({ -1.0,1.0,1.0 });

}

App::~App()
{

	return;
}


