
#include <App.h>
#include "ComputePipeLine.h"
#include "ShaderResource.h"
#include "ComputeShader.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	
	{
		ComputePipeLine gfx;

		std::vector<float> in_data = { 0,1,2,3,4 };
		std::vector<float> out_data = { 0,1,2,3,4 };

		
		ComputeShader cs(gfx, L"shaders\\ComputeShader.hlsl",false);
		ComputeShader cs2(gfx, L"shaders\\ComputeShader2.hlsl", false);

		ComputeShaderOutput<float> srOut(gfx, out_data,0);

		srOut.bind(gfx);
		cs.bind(gfx);
		gfx.getcontext()->Dispatch(1, 1, 1);
		srOut.bind(gfx);
		cs2.bind(gfx);
		gfx.getcontext()->Dispatch(1, 1, 1);

		float * data = srOut.Map(gfx);
		srOut.Unmap(gfx);

		for (short i = 0; i < 5; i++)
		{
			std::cout << i << " : " << data[i] << '\n';
		}
	}
	std::cin.get();
	FreeConsole();
	return 0;
}


