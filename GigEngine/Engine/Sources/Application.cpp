#include "Renderer.h"
#include "Application.h"
#include "Model.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "DrawLine.h"
#include "Light.h"
#include "Skybox.h"
#include "Component.h"
#include "Log.h"
#include "Behaviour.h"
#include <iostream>

#include "RigidBody.h"
#include "SceneSaver.h"
#include "WorldPhysics.h"

using namespace GigRenderer;

Application::Application()
{
	Init();

	WorldPhysics::InitPhysicWorld();
	Scene::LoadScene(defaultScene);
	CreateGameObjects();
}

Application::~Application()
{
	Lines::Clear();
	GameObjectManager::Cleanup();
	WorldPhysics::DestroyPhysicWorld();
	delete skybox;
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

void Application::Play()
{
	if (isEditor)
	{
		StartGame();
		isEditor = false;
	}
	else
	{
		Stop();
	}
}

void Application::Pause()
{
	isPause = !isPause;
}

void Application::Stop()
{
	Scene::ReloadScene(defaultScene);
	isEditor = true;
}

void Application::UseEditorCam()
{
	useEditorCam = !useEditorCam;
}

bool Application::IsInEditor()
{
	return isEditor;
}
bool Application::IsInPause()
{
	return isPause;
}
bool Application::IsUsingEditorCam()
{
	return useEditorCam;
}
void Application::StartGame()
{
	Scene::SaveScene(defaultScene);

	for (int i = 0; i < GameObjectManager::GetSize(); i++)
	{
		const GameObject* object = GameObjectManager::GetGameObject(i);

		if (object->IsActive())
		{
			for (int j = 0; j < object->GetComponentCount(); j++)
				object->GetComponentByID(j)->Start();
		}
	}
}

void Application::Run()
{
	window.ProcessInput();
	Time::UpdateDeltaTime();
	Draw();

	if (!isEditor && !isPause)
	{
		WorldPhysics::UpdatePhysics(Time::GetDeltaTime());
	}
	WorldPhysics::DrawDebug();
}

void Application::SwapFrames()
{
	window.swapBuffers();
}

void Application::Init()
{
	window.Init();
	editorCamera.SetRatio(window.GetViewPortRatio());
	RENDERER.Init();
	Lines::Init();
	InitMainShader();
}

void Application::CreateGameObjects()
{
	//to remove =====================================================

	skybox = new Skybox();
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

	RENDERER.Disable(RD_DEPTH_TEST);
	skybox->Draw();

	if (isEditor || useEditorCam)
	{
		editorCamera.Update();
	}

	mainShader.Use(); //start using the main shader

	if (!isEditor && !isPause)
	{
		UpdateGameObjectComponent(); //first because components can change the transform, destroy etc
	}

	UpdateUniforms(); //then send the global uniforms
	UpdateLights(); //send the lights to the shader (lights are gameobject, so they have been updated)

	RENDERER.Enable(RD_DEPTH_TEST);
	RENDERER.DepthFunction(RD_LESS);
	UpdateGameObjectRender(); //render model if they have one
	mainShader.UnUse(); //stop using the main shader

	Lines::DrawLines(); //render debug lines or guizmos
}

void Application::ClearWindow()
{
	RENDERER.ClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	RENDERER.Clear(RD_COLOR_BUFFER_BIT | RD_DEPTH_BUFFER_BIT);
}

void Application::UpdateGameObjectComponent()
{
	for (int i = 0; i < GameObjectManager::GetSize(); i++)
	{
		const GameObject* object = GameObjectManager::GetGameObject(i);
		object->UpdateComponents();
	}

	for (int i = 0; i < GameObjectManager::GetSize(); i++)
	{
		const GameObject* object = GameObjectManager::GetGameObject(i);
		object->LateUpdate();
	}
}

void Application::UpdateGameObjectRender()
{
	for (int i = 0; i < GameObjectManager::GetSize(); i++)
	{
		GameObject* object = GameObjectManager::GetGameObject(i);

		object->UpdateHierarchy();

		RENDERER.SetUniformValue(ModelLocation, UniformType::MAT4, lm::FMat4::ToArray(object->GetTransform().GetMatrix()));
		object->UpdateRender();
	}
}

void Application::UpdateLights()
{
	int nbDirLight = GameObjectManager::GetDirLightSize();
	int nbPointLight = GameObjectManager::GetPointLightSize();
	int nbSpotLight = GameObjectManager::GetSpotLightSize();

	RENDERER.SetUniformValue(nbDirLightLocation, UniformType::INT, &nbDirLight);
	RENDERER.SetUniformValue(nbPointLightLocation, UniformType::INT, &nbPointLight);
	RENDERER.SetUniformValue(nbSpotLightLocation, UniformType::INT, &nbSpotLight);

	GameObjectManager::SendLightsToShader();
}

void Application::UpdateUniforms() const
{
	Camera* cam = nullptr;
	if (isEditor || useEditorCam)
	{
		cam = &editorCamera;
	}
	else
	{
		if (GameObjectManager::GetCurrentCamera())
		{
			if (GameObjectManager::GetCurrentCamera()->IsActive())
			{
				cam = GameObjectManager::GetCurrentCamera();
			}
		}
	}

	if (cam)
	{
		viewProj = cam->GetProjectionMatrix() * cam->CreateViewMatrix();
		viewPos = cam->GetTransform().GetWorldPosition();
	}
	else
	{
		viewProj = lm::FMat4(0);
		viewPos = lm::FVec3(0);
	}

	RENDERER.SetUniformValue(viewProjLocation, UniformType::MAT4, lm::FMat4::ToArray(viewProj));
	RENDERER.SetUniformValue(viewPosLocation, UniformType::VEC3, &viewPos);
}
