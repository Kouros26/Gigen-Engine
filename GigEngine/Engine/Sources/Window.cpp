#include "Window.h"
#include "Inputs.h"
#include "Application.h"
#include <iostream>

void Window::Init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWmonitor* MyMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode mode = *glfwGetVideoMode(MyMonitor);

	width = mode.width;
	height = mode.height;

	window = glfwCreateWindow(width, height, APPLICATION_NAME, nullptr, nullptr);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwSetWindowPos(window, 0, 0);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, FrameBufferResizeCallback);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

void Window::ProcessInput() const
{
	Inputs::UpdateMousePosition(window);

	if (Inputs::GetKey(ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void Window::KeyCallback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
	Inputs::UpdateKey(key, action);
}

void Window::MouseButtonCallback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
	Inputs::UpdateMouseButton(button, action);
}

void Window::FrameBufferResizeCallback(GLFWwindow* pWindow, int width, int height)
{
	glViewport(0, 0, width, height);

	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
	window->width = width;
	window->height = height;
	Application::GetEditorCamera().SetRatio(window->GetRatio());
}

unsigned int Window::GetWidth() const
{
	return width;
}

unsigned int Window::GetHeight() const
{
	return height;
}

float Window::GetRatio() const
{
	if (height == 0) return 1;
	return (float)width / (float)height;
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* Window::GetGLFWWindow() const
{
	return window;
}