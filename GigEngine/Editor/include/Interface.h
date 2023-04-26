#pragma once
#include "FPSDisplay.h"
#include "FileDisplay.h"
#include "GameObjectInspector.h"
#include "HierarchyDisplay.h"
#include "MenuBarDisplay.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

struct GLFWwindow;

class Interface
{
public:

	Interface(GLFWwindow* pWindow, const char* pGlslVersion);
	~Interface();

	void BeginFrame();
	void Draw();

private:

	void SetColorDark();
	void SetFont();

	FPSDisplay fps;
	GameObjectInspector gameObjInspector;
	HierarchyDisplay hierarchy;
	FileDisplay fileDisplay;
	MenuBarDisplay menuBar;
};