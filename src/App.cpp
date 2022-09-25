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

	/* scene control */
	cameraRef.CameraMouseControl(window.mouseEvent);
	cameraRef.CameraKeyboardCotrol(window.keyBoardEvent);
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
,cameraRef(window.Gfx().getCamera())
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

	


	lamp.updateDir({ -1.0,1.0,1.0 });

}

/*
static float cx = 0, cy = 0, cz = 0, rx = 0, ry = 0;
void App::mouseHandeler() {

	cx = 0;
	cy = 0;
	cz = 0;
	rx = 0;
	ry = 0;

	cz = CameraScorollingSpeed * float(window.mouseEvent.getWheelMove());


	if (window.mouseEvent.getState() == (MK_MBUTTON | MK_SHIFT))
	{
		cx -= CameraTransilationSpeed * float(window.mouseEvent.get_dx());
		cy -= CameraTransilationSpeed * float(window.mouseEvent.get_dy());
	
		window.mouseEvent.handeled();
	}

	if (window.mouseEvent.getState() & MK_MBUTTON)
	{
		ry -= CameraRotationSpeed * float(window.mouseEvent.get_dx());
		rx += CameraRotationSpeed * float(window.mouseEvent.get_dy());
		window.mouseEvent.handeled();

	}

	// KeyBoard

	if (window.keyBoardEvent.isDown('W'))
	{
		cy = CameraTransilationSpeed;

	}
	if (window.keyBoardEvent.isDown('S'))
	{
		cy = -CameraTransilationSpeed;

	}
	if (window.keyBoardEvent.isDown('A'))
	{
		cx = CameraTransilationSpeed;

	}
	if (window.keyBoardEvent.isDown('D'))
	{
		cx = -CameraTransilationSpeed;

	}
	
	//				Camera Angle							
	
	if (window.keyBoardEvent.isDown(VK_UP))
	{
		rx = -CameraRotationSpeed;

	}
	if (window.keyBoardEvent.isDown(VK_DOWN))
	{
		rx = CameraRotationSpeed;

	}
	if (window.keyBoardEvent.isDown(VK_RIGHT))
	{
		ry = CameraRotationSpeed;

	}
	if (window.keyBoardEvent.isDown(VK_LEFT))
	{
		ry = -CameraRotationSpeed;

	}

	window.Gfx().updateCameraPosition(cx, cy, cz);
	window.Gfx().updateCameraRotation(rx, ry);
}
*/

App::~App()
{

	return;
}


