#include <windows.h>
#include "tools.h"
#include "windowManager.h"


int main(int argc, char** argv)
{
#ifndef _DEBUG
	FreeConsole();
#endif // !DEBUG

	isInDebugMode(argc, argv);
	sfVector2i defaultVideoMode = { 1920,1080 };
	Window* mainWindow = windowSetup("Super Mario Bros NES", defaultVideoMode);
	initTools();

	while (!IsDone(mainWindow))
	{
		windowUpdate(mainWindow);
		
		BeginDraw(mainWindow);
		windowDraw(mainWindow);
		EndDraw(mainWindow);
	}
	return 0;
}