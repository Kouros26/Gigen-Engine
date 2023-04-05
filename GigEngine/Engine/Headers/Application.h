#pragma once
#include "Shader.h"
#include "Window.h"
#include "EditorCamera.h"

class Application
{
public:
    Application();
    ~Application();

    static Window& GetWindow();
    static EditorCamera& GetEditorCamera();
    static ShaderProgram& GetMainShader();

    static lm::FMat4& GetViewProj();
    static lm::FVec3& GetViewPos();

    static void StartGame();
    void Run();
    void SwapFrames();

private:
    static inline Window window;
    static inline EditorCamera editorCamera;

    static inline lm::FMat4 viewProj;
    static inline lm::FVec3 viewPos;

    bool isEditor = true;

    //main shader
    static inline ShaderProgram mainShader;
    int viewProjLocation;
    int ModelLocation;
    int viewPosLocation;
    int nbDirLightLocation;
    int nbPointLightLocation;
    int nbSpotLightLocation;

    void InitGLFW();

    void InitMainShader();
    void Draw();
    void ClearWindow();
    void UpdateGameObjectComponent();
    void UpdateGameObjectRender();
    void UpdateLights();
    void UpdateUniforms();
};
