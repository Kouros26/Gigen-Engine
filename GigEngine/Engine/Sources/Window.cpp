#include "Renderer.h"
#include "Window.h"
#include "Inputs.h"
#include "Application.h"
#include <iostream>
#include <stb_image.h>

using namespace GigRenderer;

void Window::Init()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, RD_TRUE);

	GLFWmonitor* MyMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode mode = *glfwGetVideoMode(MyMonitor);

	width = mode.width;
	height = mode.height;

	//if I put my monitor here, can't debug
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
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	ToggleVSync(1);

	version = "#version ";
	version += std::to_string(versionMajor);
	version += std::to_string(versionMinor);
	version += '0';

	SetIcon("Engine/Textures/Icon.png");
}

void Window::ProcessInput() const
{
	glfwPollEvents();
	GigInput::Inputs::UpdateMousePosition();

	//if (GigInput::Inputs::GetKey(GigInput::Keys::ESCAPE))
		//here stop running in editor ok
}

void Window::Close() const
{
	glfwSetWindowShouldClose(window, true);
}

void Window::SetIcon(const std::string& pPath) const
{
	GLFWimage images[1];
	images[0].pixels = stbi_load(pPath.c_str(), &images[0].width, &images[0].height, 0, 4); //rgba channels
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
}

void Window::KeyCallback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
	GigInput::Inputs::UpdateKey(key, action);
}

void Window::MouseButtonCallback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
	GigInput::Inputs::UpdateMouseButton(button, action);
}

void Window::FrameBufferResizeCallback(GLFWwindow* pWindow, int width, int height)
{
	RENDERER.ViewPort(0, 0, width, height);

	const auto window = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
	window->width = width;
	window->height = height;
	Application::GetEditorCamera().SetRatio(window->GetViewPortRatio());
}

void Window::scrollCallback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
	GigInput::Inputs::UpdateMouseWheelOffset(yoffset);
}

void Window::setCursorShow(bool pShowCursor) const
{
	if (pShowCursor)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Window::swapBuffers() const
{
	glfwSwapBuffers(window);
}

void Window::getCursorPosition(double& xpos, double& ypos) const
{
	glfwGetCursorPos(window, &xpos, &ypos);
}

void Window::SetMouseIcon(CursorShape shape) const
{
	GLFWcursor* cursor;
	switch (shape)
	{
	case CursorShape::BEAM:
		cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		break;
	case CursorShape::CROSSHAIR:
		cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		break;
	case CursorShape::HAND:
		cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
		break;
	default:
		cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		break;
	}
	glfwSetCursor(window, cursor);
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

unsigned int Window::GetVPWidth() const
{
	return vpWidth;
}

unsigned int Window::GetVPHeight() const
{
	return vpHeight;
}

unsigned int Window::GetVPX() const
{
	return vpPosx;
}

unsigned int Window::GetVPY() const
{
	return vpPosy;
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

float Window::GetViewPortRatio() const
{
	return viewPortRatio;
}

void Window::SetViewPort(unsigned int pX, unsigned int pY, unsigned int pWidth, unsigned int pHeight)
{
	if (pX == vpPosx && pY == vpPosy && pWidth == vpWidth && pHeight == vpHeight) return;

	RENDERER.ViewPort(pX, pY, pWidth, pHeight);

	vpPosx = pX;
	vpPosy = pY;
	vpWidth = pWidth;
	vpHeight = pHeight;

	viewPortRatio = static_cast<float>(pWidth) / static_cast<float>(pHeight);
	Application::GetEditorCamera().SetRatio(viewPortRatio);
}