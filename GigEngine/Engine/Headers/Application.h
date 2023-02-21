#pragma once
#include "Window.h"

class Application
{
public:
	Application();
	~Application();

	static Window& GetWindow();

	void Run();
private:
	static inline Window window;

	void InitOpenGl();
	void InitGlad();
	void Draw();
};
