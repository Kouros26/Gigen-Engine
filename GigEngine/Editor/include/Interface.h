#pragma once
#include "FPSDisplay.h"
#include "ToolsDisplay.h"
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
	static void Draw();

private:

	void SetColorDark();
	void SetFont();

	GameObjectInspector gameObjInspector;
	HierarchyDisplay hierarchy;
	ToolsDisplay fileDisplay;
	MenuBarDisplay menuBar;
};