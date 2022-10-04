
#include <App.h>
#include "ShaderResource.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	
	{

		float in_data[5] = { 0,1,2,3,4 };
		float out_data[5] = { 0,1,2,3,4 };


		App app;

		ComputeShader cs(app.gfx, L"shaders\\ComputeShader.hlsl",false);
		ShaderResource srIn(app.gfx, in_data, sizeof(in_data), DXGI_FORMAT_R32_FLOAT, COMPUTE_SHADER_STAGE, 0u, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		ShaderResource srOut(app.gfx, in_data, sizeof(in_data), DXGI_FORMAT_R32_FLOAT, COMPUTE_SHADER_STAGE, 1u, D3D11_USAGE_STAGING, D3D11_CPU_ACCESS_READ);

		cs.bind(app.gfx);
		srIn.bind(app.gfx);
		srOut.bind(app.gfx);

		app.gfx.getcontext()->Dispatch(1, 0, 0);
		app.go();

	}
	std::cin.get();
	FreeConsole();
	return 0;
}


