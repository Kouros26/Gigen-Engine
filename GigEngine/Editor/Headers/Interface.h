#pragma once
#include "FPSDisplay.h"
#include "FileDisplay.h"
#include "GameObjectInspector.h"
#include "HierarchyDisplay.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Interface
{
public:

	Interface(GLFWwindow* window, const char* glslVersion);
	~Interface();

	void BeginFrame();
	void Draw();

private:

	FPSDisplay fps;
	GameObjectInspector gameObjInspector;
	HierarchyDisplay hierarchy;
	FileDisplay fileDisplay;
};