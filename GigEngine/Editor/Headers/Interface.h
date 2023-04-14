#pragma once
#include "FPSDisplay.h"
#include "GameObjectInspector.h"

struct GLFWwindow;

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
};