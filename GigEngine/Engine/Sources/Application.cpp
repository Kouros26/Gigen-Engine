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
	GameObject* first = new GameObject();
	GameObject* second = new GameObject();
	base->GetTransform().SetWorldScale(lm::FVec3(0.01f));
	first->GetTransform().SetWorldScale({ 0.5f });
	second->GetTransform().SetWorldScale({ 0.5f });
	second->GetTransform().SetWorldPosition({ 0,1,0 });
	base->SetModel("Resources/Models/sponza.obj");
	first->SetModel("Resources/Models/chest.obj");
	second->SetModel("Resources/Models/chest.obj");
	first->AddChild(second);

	//base->AddNewComponent<testComponent>();
	GameObjectManager::AddGameObject(base);
	GameObjectManager::AddGameObject(first);
	GameObjectManager::AddGameObject(second);
	Lines::SetFocusedObjectTransform(&base->GetTransform());

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
			glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, lm::FMat4::ToArray(object->GetTransform().GetMatrix()));
			object->UpdateRender();
		}
	}
}

void Application::UpdateUniforms()
{
	mainShader.Use();

	viewProj = editorCamera.GetProjectionMatrix() * editorCamera.CreateViewMatrix();
	viewPos = editorCamera.GetTransform().GetWorldPosition();

	glUniformMatrix4fv(viewProjLocation, 1, GL_FALSE, lm::FMat4::ToArray(viewProj));
	glUniform3f(viewPosLocation, viewPos.x, viewPos.y, viewPos.z);
}