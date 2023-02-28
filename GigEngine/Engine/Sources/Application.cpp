#include "Application.h"
#include "Inputs.h"
#include "Watch.h"
#include "Model.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "DrawLine.h"
#include "Light.h"
#include <iostream>

Application::Application()
{
	InitOpenGl();
	window.Init();
	editorCamera.SetRatio(window.GetRatio());
	InitGlad();
	Lines::Init();
	InitMainShader();

	//to remove =====================================================
	GameObject* base = new GameObject();
	base->transform.SetScale(lm::FVec3(0.01f));
	base->setModel("Resources/Models/sponza.obj");
	base->AddNewComponent<TestComponent>();
	GameObjectManager::AddGameObject(base);
	Lines::SetFocusedObjectTransform(&base->transform);

	DirLight* dirlight = new DirLight(0.1f, 0.2f, 0.3f, lm::FVec3(1,0,0));
	GameObjectManager::AddGameObject(dirlight);

	//SpotLight* spotlight = new SpotLight(0.1f, 0.2f, 0.3f, 0.2f, 0.2f, 0.2f, 45, 90, lm::FVec3(0, 0, 1));
	//GameObjectManager::AddGameObject(spotlight);

	Lines::DrawLine(lm::FVec3(1, 0, 5), lm::FVec3(1, 11, 5), lm::FVec3(0, 0, 1), 5);
	Lines::DrawLine(lm::FVec3(2, 0, 5), lm::FVec3(2, 12, 5), lm::FVec3(0, 1, 1), 6);
	Lines::DrawLine(lm::FVec3(3, 0, 5), lm::FVec3(3, 13, 5), lm::FVec3(1, 1, 1), 7);
	Lines::DrawLine(lm::FVec3(4, 0, 5), lm::FVec3(4, 14, 5), lm::FVec3(1, 0, 1), 8);
	Lines::DrawLine(lm::FVec3(5, 0, 5), lm::FVec3(5, 15, 5), lm::FVec3(1, 1, 0), 9);
	//==================================================================
}

Application::~Application()
{
	Lines::Clear();
	GameObjectManager::Cleanup();
}

Window& Application::GetWindow()
{
	return window;
}

EditorCamera& Application::GetEditorCamera()
{
	return editorCamera;
}

ShaderProgram& Application::GetMainShader()
{
	return mainShader;
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

void Application::InitMainShader()
{
	VertexShader* mainVertex = ResourceManager::Get<VertexShader>("Resources/Shaders/vert.vert");
	FragmentShader* mainFragment = ResourceManager::Get<FragmentShader>("Resources/Shaders/frag.frag");

	if (!mainShader.Link(mainVertex, mainFragment))
		std::cout << "Error linking main shader" << std::endl;

	ModelLocation = glGetUniformLocation(mainShader.GetId(), "model");
	viewProjLocation = glGetUniformLocation(mainShader.GetId(), "viewProj");
	viewPosLocation = glGetUniformLocation(mainShader.GetId(), "viewPos");

	nbDirLightLocation = glGetUniformLocation(mainShader.GetId(), "nbDirLight");
	nbPointLightLocation = glGetUniformLocation(mainShader.GetId(), "nbPointLight");
	nbSpotLightLocation = glGetUniformLocation(mainShader.GetId(), "nbSpotLight");
}

void Application::Draw()
{
	ClearWindow();

	if (isEditor)
	{
		editorCamera.Update();

		UpdateGameObjectComponent(); //first because components can change the transform, destroy etc
		UpdateUniforms(); //then send the global uniforms
		UpdateLights(); //send the lights to the shader (lights are gameobject, so they have been updated)
		
		glEnable(GL_DEPTH_TEST);
		UpdateGameObjectRender(); //render model if they have one
		Lines::DrawLines(); //render debug lines or guizmos
	}
}

void Application::ClearWindow()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::UpdateGameObjectComponent()
{
	for (int i = 0; i < GameObjectManager::GetSize(); i++)
	{
		GameObject* object = GameObjectManager::GetGameObject(i);
		object->UpdateComponents();
	}
}

void Application::UpdateGameObjectRender()
{
	for (int i = 0; i < GameObjectManager::GetSize(); i++)
	{
		GameObject* object = GameObjectManager::GetGameObject(i);

		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, lm::FMat4::ToArray(object->transform.GetMatrix()));
		object->UpdateRender();
	}
}

void Application::UpdateLights()
{
	glUniform1i(nbDirLightLocation, GameObjectManager::GetDirLightSize());
	glUniform1i(nbPointLightLocation, GameObjectManager::GetPointLightSize());
	glUniform1i(nbSpotLightLocation, GameObjectManager::GetSpotLightSize());

	GameObjectManager::SendLightsToShader();
}

void Application::UpdateUniforms()
{
	mainShader.Use();

	viewProj = editorCamera.GetProjectionMatrix() * editorCamera.CreateViewMatrix();
	viewPos = editorCamera.transform.GetPosition();

	glUniformMatrix4fv(viewProjLocation, 1, GL_FALSE, lm::FMat4::ToArray(viewProj));
	glUniform3f(viewPosLocation, viewPos.x, viewPos.y, viewPos.z);
}