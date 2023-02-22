#pragma once
#include "Window.h"
#include "EditorCamera.h"

class Application
{
public:
    Application();
    ~Application();

    static Window& GetWindow();
    static EditorCamera& GetEditorCamera();

    void Run();
private:
    static inline Window window;
    static  inline  EditorCamera editorCamera;

    bool isEditor = true;

    void InitOpenGl();
    void InitGlad();
    void Draw();
};
