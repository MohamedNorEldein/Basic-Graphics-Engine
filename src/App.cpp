//
// Created by m7mdn on 7/23/2022.
//
#include <time.h>
#include "App.h"

WPARAM App::go() {

	MSG msg;
	do {
		PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
		//while (GetMessage(&msg, nullptr, 0, 0)) {

		doFrame();
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		//+++++++

	} while (msg.message != WM_QUIT);


	return msg.wParam;

}
COLOR c;

WPARAM App::doFrame() {
	/* window begain frame stuff*/

	window.Gfx().clearBuffer(0.5, 0.5, 0.5);
	window.Gfx().BegainFrame();
	
	lamp.bind(window.Gfx());
	lamp.GuiControl();

	mouseHandeler();
	
	c = { 1.0,0.50,0.20 };
	model1.GuiControl();
	model1.pcb->update(window.Gfx(), c);
	model1.Draw();

	/*ImGui stuff*/
	updateView();


	/*window end frame stuff */
	window.Gfx().EndFrame();
	return 0;
}

App::App() : window(L"hello world", 1200, 600)
	, lamp(window.Gfx(), 0u)
	//, model1(window.Gfx())
	, model1(window.Gfx(),"Models src data\\nanosuit.obj")
{
	 
	window.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5, 10000.0f));

	model1.setPos(0, 0, 100);
	model1.setDiminsion(100, 100, 100);
	
	lamp.updateDir({ -1.0,1.0,1.0 });
	lamp.updateColor({ 1.0,0,0 });

}

void App::updateView()
{


	if (ImGui::Begin("Camera")) {
		ImGui::SliderFloat2("camera position:", v, -5000.0f, +5000.0f);
		ImGui::SliderFloat2("camera Rotation:", u, -5.0f, +5.0f);

		window.Gfx().updateCameraPosition(v[0], v[1]);
		window.Gfx().updateCameraRotation(u[0], u[1]);
		if (ImGui::Button("reset", ImVec2(50, 25))) {
			v[0] = { 0 };
			v[1] = { 0 };

			u[0] = { 0 };
			u[1] = { 0 };
			
		}

	}
	ImGui::End();
}


void App::mouseHandeler() {



}


App::~App()
{
	std::cout << "delete app\n";
	

	return;
}


