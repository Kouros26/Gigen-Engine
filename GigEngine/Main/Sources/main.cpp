#define _CRTDBG_MAP_ALLOC
#include "Application.h"
#include "Interface.h"

int main()
{
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
		std::cout << "-----------_CrtMemDumpStatistics ---------" << std::endl;
		_CrtMemDumpStatistics(&sDiff);
		std::cout << "-----------_CrtMemDumpAllObjectsSince ---------" << std::endl;
		_CrtMemDumpAllObjectsSince(&sOld);
		std::cout << "-----------_CrtDumpMemoryLeaks ---------" << std::endl;
		_CrtDumpMemoryLeaks();
	}
}