#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	void Init();
	void ProcessInput() const;
	static void KeyCallback(GLFWwindow*, int key, int, int action, int);
	static void MouseButtonCallback(GLFWwindow*, int button, int action, int);
	static void FrameBufferResizeCallback(GLFWwindow* pWindow, int width, int height);

	[[nodiscard]] bool ShouldClose() const;
	[[nodiscard]] GLFWwindow* GetGLFWWindow() const;

private:

	const char* APPLICATION_NAME = "Gigen Engine";

	GLFWwindow* window = nullptr;
	unsigned int width{};
	unsigned int height{};
};
