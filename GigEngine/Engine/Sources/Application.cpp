#include "Renderer.h"
#include "Application.h"
#include "Model.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "DrawLine.h"
#include "Light.h"
#include "Skybox.h"
#include "Component.h"
#include <iostream>
#include "WorldPhysics.h"

using namespace GigRenderer;

Application::Application()
{
    Init();

    WorldPhysics::InitPhysicWorld();
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

bool Application::IsInEditor()
{
    return isEditor;
}
void Application::StartGame()
{
    for (int i = 0; i < GameObjectManager::GetSize(); i++)
    {
        const GameObject* object = GameObjectManager::GetGameObject(i);

        for (int j = 0; j < object->GetComponentCount(); j++)
            object->GetComponentByID(j)->Start();
    }
}

void Application::Run()
{
    window.ProcessInput();
    Time::UpdateDeltaTime();
    Draw();
    WorldPhysics::UpdatePhysics(Time::GetDeltaTime());
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

    GameObject* chest = GameObjectManager::CreateGameObject("chest", { 5, 10, 10 }, { 0 }, { 1 });
    chest->SetModel("Resources/Models/chest.obj");
    chest->SetTexture("Resources/Textures/test.png");

    GameObject* car = GameObjectManager::CreateGameObject("car", { -5, 10, 10 }, { 0 }, { 1 });
    car->SetModel("Resources/Models/Car.fbx");
    //car->AddComponent<TestComponent>();
    car->AddComponent<testComponent2>();
    Lines::SetFocusedObjectTransform(&car->GetTransform());
    car->AddChild(chest);
    car->CreateCapsuleRigidBody(1, 5, {1}, 10);

    GameObject* ground = GameObjectManager::CreateGameObject("Ground");
    ground->SetModel("Resources/Models/Basics/Cube.FBX");
    ground->GetTransform().SetWorldScale({ 1, 0.05f, 0.5f });
    ground->CreateBoxRigidBody({ 1 }, { 50 }, 0.f);

    GameObject* TH = GameObjectManager::CreateGameObject("Thierry-Henri", {-5, 15, 8}, { 0, 90, 0 }, {20});
    TH->SetModel("Resources/Models/Thierry-Henri.obj");
    TH->CreateSphereRigidBody(2, { 0.05f }, 10.0f);

    //chest->CreateBoxRigidBody({ 10 }, { 1 }, 10.f);

    GameObject* dirlight = GameObjectManager::CreateDirLight(0.5f, 0.5f, 0.7f, lm::FVec3(1));
    dirlight->GetTransform().SetWorldRotation(lm::FVec3(45, 20, 0));
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
        RENDERER.Disable(RD_DEPTH_TEST);
        skybox->Draw();
        editorCamera.Update();
        mainShader.Use(); //start using the main shader

        UpdateGameObjectComponent(); //first because components can change the transform, destroy etc
        UpdateUniforms(); //then send the global uniforms
        UpdateLights(); //send the lights to the shader (lights are gameobject, so they have been updated)

        RENDERER.Enable(RD_DEPTH_TEST);
        RENDERER.DepthFunction(RD_LESS);
        UpdateGameObjectRender(); //render model if they have one
        mainShader.UnUse(); //stop using the main shader

        Lines::DrawLines(); //render debug lines or guizmos
    }
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

void Application::UpdateUniforms()
{
    viewProj = editorCamera.GetProjectionMatrix() * editorCamera.CreateViewMatrix();
    viewPos = editorCamera.GetTransform().GetWorldPosition();

    RENDERER.SetUniformValue(viewProjLocation, UniformType::MAT4, lm::FMat4::ToArray(viewProj));

    RENDERER.SetUniformValue(viewPosLocation, UniformType::VEC3, &viewPos);
}