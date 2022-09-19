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
color c;

WPARAM App::doFrame() {
	/* window begain frame stuff*/

	window.Gfx().clearBuffer(0.5, 0.5, 0.5);
	window.Gfx().BegainFrame();
	
	sun.bind(window.Gfx());
	mouseHandeler();
	
	
	model1.GuiControl("model 1");

	
	model1.pPCbuff->update(window.Gfx(), c);
	model1.Draw(window.Gfx());

	c = { 0.0f,1.0f,0.0f };
	model2.pPCbuff->update(window.Gfx(), c);
	model2.Draw(window.Gfx());
	model2.GuiControl("model 2");
	
	/*ImGui stuff*/
	updateView();


	/*window end frame stuff */
	window.Gfx().EndFrame();
	return 0;
}

App::App() : window(L"hello world", 1200, 600)
	, sun(window.Gfx(), 0u)
	, model1(window.Gfx()), model2(window.Gfx())
{
	 
	window.Gfx().setProjection(
		DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5, 10000.0f)
	);
	
	//GCLASS* pMonkey_GCLASS = GenerateClassFromFile(window.Gfx(), "D:\\computer\\directX\\Project1\\Models\\monkey.obj");
	GCLASS* pMonkey_GCLASS = GenerateClassFromFile(window.Gfx(), "Models src data\\monkey.obj");
	GCLASS* pCube_GCLASS = GenerateClassFromFile(window.Gfx(), "Models src data\\bugatti.obj");

	
	model1.setGCLASS(pCube_GCLASS);
	model1.pPCbuff = new PixelConstantBuffer<color>(window.Gfx(),c , 1u);
	model1.getGCLASS().AddBindable(model1.pPCbuff);

	model1.setDiminsion(100, 100, 100);
	model1.setPos(0, 0, 1000);

	model2.setGCLASS(pMonkey_GCLASS->getClassName());
	model2.pPCbuff = model1.pPCbuff;

	model2.setDiminsion(100, 100, 100);
	model2.setPos(500, 0, 1000);
	
	sun.updateDir({ -10,10,0 });
	sun.updateColor({ 1.0,0,0 });

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


