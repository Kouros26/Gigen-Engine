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

	GameObject* chest = GameObjectManager::CreateGameObject();
	chest->setModel("Resources/Models/chest.obj");
	chest->AddComponent<TestComponent>();
	chest->transform.SetPosition(lm::FVec3(5, 0, 10));

	GameObject* car = GameObjectManager::CreateGameObject();
	car->setModel("Resources/Models/Car.fbx");
	car->AddComponent<TestComponent>();
	car->transform.SetPosition(lm::FVec3(-5, 0, 10));
	Lines::SetFocusedObjectTransform(&car->transform);

	GameObject* dirlight = GameObjectManager::CreateDirLigth(0.05f, 0.2f, 0.5f, lm::FVec3(1));
	dirlight->transform.SetRotation(lm::FVec3(45, 20, 0));

	GameObject* dirlight2 = GameObjectManager::CreateDirLigth(0.05f, 0.2f, 0.3f, lm::FVec3(1));
	dirlight2->transform.SetRotation(lm::FVec3(-45, -20, 0));

	GameObject* pointlight = GameObjectManager::CreatePointLight(0.05f, 0.2f, 0.3f, 0.01f, 0.01f, 0.01f, lm::FVec3(1));
	pointlight->transform.SetPosition(lm::FVec3(10, 0, 10));

	//GameObject* spotlight = GameObjectManager::CreateSpotLight(0.05f, 0.2f, 0.5f, 0.01f, 0.01f, 0.01f, 25, 50, lm::FVec3(1));
	//spotlight->transform.SetRotation(lm::FVec3(90, 0, 0));
	//spotlight->transform.SetPosition(lm::FVec3(-5, 10, 10));

	//GameObject* spotlight2 = GameObjectManager::CreateSpotLight(0.1f, 0.1f, 0.2f, 0.01f, 0.01f, 0.01f, 25, 50, lm::FVec3(1));
	//spotlight2->transform.SetRotation(lm::FVec3(-90, 0, 0));
	//spotlight2->transform.SetPosition(lm::FVec3(-5, -10, 10));

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
	VertexShader* mainVertex = ResourceManager::Get<VertexShader>("Resources/Shaders/core_vert.vert");
	FragmentShader* mainFragment = ResourceManager::Get<FragmentShader>("Resources/Shaders/core_frag.frag");

	if (!mainShader.Link(mainVertex, mainFragment))
		std::cout << "Error linking main shader" << std::endl;

	ModelLocation = mainShader.GetUniform("model");
	viewProjLocation = mainShader.GetUniform("viewProj");
	viewPosLocation = mainShader.GetUniform("viewPos");

	nbDirLightLocation = mainShader.GetUniform("nbDirLight");
	nbPointLightLocation = mainShader.GetUniform("nbPointLight");
	nbSpotLightLocation = mainShader.GetUniform("nbSpotLight");
}

void Application::Draw()
{
	ClearWindow();

	if (isEditor)
	{
		mainShader.Use(); //start using the main shader
		editorCamera.Update();
		UpdateGameObjectComponent(); //first because components can change the transform, destroy etc
		UpdateUniforms(); //then send the global uniforms
		UpdateLights(); //send the lights to the shader (lights are gameobject, so they have been updated)

		glEnable(GL_DEPTH_TEST);
		UpdateGameObjectRender(); //render model if they have one
		mainShader.UnUse(); //stop using the main shader

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