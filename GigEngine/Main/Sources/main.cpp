#define _CRTDBG_MAP_ALLOC
#include "Application.h"
#include "Interface.h"
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtMemState sOld;
	_CrtMemState sNew;
	_CrtMemState sDiff;
	_CrtMemCheckpoint(&sOld); //take a snapshot
	Application app;
	Window& window = Application::GetWindow(); //Used as text replacement

	Interface interface(window.GetGLFWWindow(), window.GetGLSLVersion().c_str());

	while (!window.ShouldClose())
	{
		interface.BeginFrame();
		app.Run();
		interface.Draw();
		app.SwapFrames();
	}

	_CrtMemCheckpoint(&sNew); //take a snapshot
	if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
	{
		_CrtMemDumpStatistics(&sDiff);
		_CrtMemDumpAllObjectsSince(&sOld);
		_CrtDumpMemoryLeaks();
	}
}