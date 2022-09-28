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

	window.Gfx().clearBuffer(0.5, 0.5, 0.5);
	window.Gfx().BegainFrame();
	window.Gfx().getCamera().GUIcontrol();

	lamp.bind(window.Gfx());
	lamp.GuiControl();


	c = { 1,0,0 };
	
	model1.pcb->update(window.Gfx(), c);
	model1.Draw();

	c = { 0,1.0,0 };
	model2.pcb->update(window.Gfx(), c);
	model2.Draw();

	c = { 0,0,1 };
	model3.pcb->update(window.Gfx(), c);
	model3.Draw();

	c = { 0,1,1 };
	model4.pcb->update(window.Gfx(), c);
	model4.Draw();
	
	objModel.pcb->update(window.Gfx(), c);
	objModel.Draw();
	objModel.GuiControl();
	
	/* scene control */
	window.Gfx().getCamera().CameraMouseControl(window.mouseEvent);
	window.Gfx().getCamera().CameraKeyboardCotrol(window.keyBoardEvent);
	/*window end frame stuff */
	window.Gfx().EndFrame();
	return 0;
}

App::App() : window(L"hello world", 1200, 600)
, lamp(window.Gfx(), 0u)
, model1(window.Gfx())
, model2(window.Gfx())
, model3(window.Gfx())
, model4(window.Gfx())
,objModel(window.Gfx(), "Models src data\\moonkey.glb")
{

	window.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5, 10000.0f));
	
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


