#include <App.h>
#include "AugmantedTree.h"
#include "mstring.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	
	{
		App app;
		app.go();

	}

		
	
	std::cin.get();
	FreeConsole();
	return 0;
}


