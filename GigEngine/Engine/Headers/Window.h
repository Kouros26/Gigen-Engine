#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	void Init(int major, int minor);
	void ProcessInput() const;

	static void KeyCallback(GLFWwindow*, int key, int, int action, int);
	static void MouseButtonCallback(GLFWwindow*, int button, int action, int);
	static void FrameBufferResizeCallback(GLFWwindow* pWindow, int width, int height);

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	float GetRatio() const;

	[[nodiscard]] bool ShouldClose() const;
	[[nodiscard]] GLFWwindow* GetGLFWWindow() const;
	std::string& GetGLSLVersion();

private:

	const char* APPLICATION_NAME = "Gigen Engine";

	GLFWwindow* window = nullptr;
	std::string version;

	unsigned int width = 0;
	unsigned int height = 0;
};
