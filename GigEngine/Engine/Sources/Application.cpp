#include "Application.h"
#include "Inputs.h"
#include "Watch.h"
#include "Shader.h"
#include <iostream>

Application::Application()
{
	InitOpenGl();
	window.Init();
	InitGlad();
}

Application::~Application()
{
}

Window& Application::GetWindow()
{
	return window;
}

void Application::Run()
{
	//test
	VertexShader vertex("Assets/Shaders/vert.vert");
	FragmentShader fragment("Assets/Shaders/frag.frag");

	Shader::Link(vertex, fragment);

	while (!window.ShouldClose())
	{
		glfwPollEvents();
		window.ProcessInput();
		Time::UpdateDeltaTime();
		Draw();
		glfwSwapBuffers(window.GetGLFWWindow());
	}
}

void Application::InitOpenGl()
{
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize GLFW" << std::endl;
	}
}

void Application::InitGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
	}
}

void Application::Draw()
{
	Shader::Use();
	//clear
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}