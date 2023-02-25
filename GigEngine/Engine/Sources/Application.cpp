#include "Application.h"
#include "Inputs.h"
#include "Watch.h"
#include "Shader.h"
#include "Model.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "DrawLine.h"
#include <iostream>

//to remove when resource manager
ShaderProgram mainShader;
GLint viewProjLocation;
GLint ModelLocation;
GLint viewPosLocation;

Application::Application()
{
	InitOpenGl();
	window.Init();
	editorCamera.SetRatio(window.GetRatio());
	InitGlad();
	Lines::Init();

	//to remove =====================================================
	VertexShader* mainVertex = ResourceManager::Get<VertexShader>("Resources/Shaders/vert.vert");
	FragmentShader* mainFragment = ResourceManager::Get<FragmentShader>("Resources/Shaders/frag.frag");

	mainShader.Link(mainVertex, mainFragment);

	viewProjLocation = glGetUniformLocation(mainShader.GetId(), "viewProj");
	ModelLocation = glGetUniformLocation(mainShader.GetId(), "model");
	viewPosLocation = glGetUniformLocation(mainShader.GetId(), "viewPos");

	GameObject* base = new GameObject();
	base->transform.SetScale(lm::FVec3(0.01f));
	base->setModel("Resources/Models/sponza.obj");
	base->AddNewComponent<testComponent>();
	GameObjectManager::AddGameObject(base);

	Lines::AddLine(lm::FVec3(1, 0, 0), lm::FVec3(0, 10, 0));
	Lines::AddLine(lm::FVec3(10, 0, 0), lm::FVec3(0, 5, 0));
	Lines::AddLine(lm::FVec3(0, 0, 10), lm::FVec3(0, -10, 0));
	Lines::AddLine(lm::FVec3(1, 0, -10), lm::FVec3(0, 10, 3));
	//==================================================================
}

Application::~Application()
{
	Lines::Clear();
}

Window& Application::GetWindow()
{
	return window;
}

EditorCamera& Application::GetEditorCamera()
{
	return editorCamera;
}

lm::FMat4& Application::GetViewProj()
{
	return viewProj;
}

lm::FVec3& Application::GetViewPos()
{
	return viewPos;
}

void Application::Run()
{
	glfwPollEvents();
	window.ProcessInput();
	Time::UpdateDeltaTime();
	Draw();
}

void Application::SwapFrames()
{
	glfwSwapBuffers(window.GetGLFWWindow());
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
	ClearWindow();

	if (isEditor)
	{
		editorCamera.Update();
		UpdateUniforms();

		glEnable(GL_DEPTH_TEST);
		UpdateGameObjects();

		glDisable(GL_DEPTH_TEST);
		Lines::DrawLines();
	}
}

void Application::ClearWindow()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::UpdateGameObjects()
{
	for (int i = 0; i < GameObjectManager::GetSize(); i++)
	{
		GameObject* object = GameObjectManager::GetGameObject(i);

		if (object)
		{
			object->UpdateComponents();
			glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, lm::FMat4::ToArray(object->transform.GetMatrix()));
			object->UpdateRender();
		}
	}
}

void Application::UpdateUniforms()
{
	mainShader.Use();

	viewProj = editorCamera.GetProjectionMatrix() * editorCamera.CreateViewMatrix();
	viewPos = editorCamera.transform.GetPosition();

	glUniformMatrix4fv(viewProjLocation, 1, GL_FALSE, lm::FMat4::ToArray(viewProj));
	glUniform3f(viewPosLocation, viewPos.x, viewPos.y, viewPos.z);
}