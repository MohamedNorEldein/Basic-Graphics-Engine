#include <App.h>
#include <List.h>
#include <VertexBuffer.h>



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	
	{
		App app;
		app.go();

	}
	
	cout(sin(0));
	
	std::cin.get();
	FreeConsole();
	return 0;
}


