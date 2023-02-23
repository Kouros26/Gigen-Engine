#include "Window.h"
#include "Inputs.h"
#include "Application.h"
#include <iostream>

void Window::Init(int major, int minor)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWmonitor* MyMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode mode = *glfwGetVideoMode(MyMonitor);

	width = mode.width;
	height = mode.height;

	window = glfwCreateWindow(width, height, APPLICATION_NAME, nullptr, nullptr);

	if (window == nullptr)
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
	ToggleVSync(1);

	version = "#version ";
	version += std::to_string(major);
	version += std::to_string(minor);
	version += '0';
}

void Window::ProcessInput() const
{
	Inputs::UpdateMousePosition(window);

	if (Inputs::GetKey(ESCAPE))
		glfwSetWindowShouldClose(window, true);
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

	const auto window = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
	window->width = width;
	window->height = height;
	Application::GetEditorCamera().SetRatio(window->GetRatio());
}

void Window::ToggleVSync(int input)
{
	glfwSwapInterval(input);
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
	return static_cast<float>(width) / static_cast<float>(height);
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* Window::GetGLFWWindow() const
{
	return window;
}

std::string& Window::GetGLSLVersion()
{
	return version;
}