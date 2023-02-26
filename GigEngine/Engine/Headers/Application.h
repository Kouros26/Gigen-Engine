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

	static lm::FMat4& GetViewProj();
	static lm::FVec3& GetViewPos();

	void Run();
	void SwapFrames();

private:
	static inline Window window;
	static inline EditorCamera editorCamera;

	static inline lm::FMat4 viewProj;
	static inline lm::FVec3 viewPos;

	bool isEditor = true;

	void InitOpenGl();
	void InitGlad();
	void Draw();
	void ClearWindow();
	void UpdateGameObjects();
	void UpdateUniforms();
};
