#include "Application.h"
#include "Inputs.h"
#include "Watch.h"
#include "Shader.h"
#include "Model.h"
#include "GameObjectManager.h"
#include <iostream>

//to remove when resource manager
ShaderProgram mainShader;
GLint viewProjLocation;
GLint ModelLocation;
GLint viewPosLocation;

Application::Application()
{
    InitOpenGl();
    window.Init(3,3);
    editorCamera.SetRatio(window.GetRatio());
    InitGlad();

    //to remove =====================================================
    VertexShader mainVertex("Resources/Shaders/vert.vert");
    FragmentShader mainFragment("Resources/Shaders/frag.frag");

    mainShader.Link(mainVertex, mainFragment);

    viewProjLocation = glGetUniformLocation(mainShader.GetId(), "viewProj");
    ModelLocation = glGetUniformLocation(mainShader.GetId(), "model");
    viewPosLocation = glGetUniformLocation(mainShader.GetId(), "viewPos");

    GameObject* base = new GameObject();
    Model* model = new Model(base, "Resources/Models/sponza.obj");
    base->transform.SetScale(lm::FVec3(0.01f));
    base->AddComponent(model);
    testComponent* test = new testComponent(base);
    base->AddComponent(test);
    GameObjectManager::AddGameObject(base);
    //==================================================================

    glEnable(GL_DEPTH_TEST);
}

Application::~Application()
{
}

Window& Application::GetWindow()
{
    return window;
}

EditorCamera& Application::GetEditorCamera()
{
    return editorCamera;
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
        UpdateGameObjects();
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
            glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, lm::FMat4::ToArray(object->transform.GetMatrix()));
            object->Update();
        }
    }
}

void Application::UpdateUniforms()
{
    mainShader.Use();

    lm::FMat4 viewProj = editorCamera.GetProjectionMatrix() * editorCamera.CreateViewMatrix();
    lm::FVec3 viewPos = editorCamera.transform.GetPosition();

    glUniformMatrix4fv(viewProjLocation, 1, GL_FALSE, lm::FMat4::ToArray(viewProj));
    glUniform3f(viewPosLocation, viewPos.x, viewPos.y, viewPos.z);
}
