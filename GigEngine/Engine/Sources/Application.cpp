#include "Application.h"
#include "Inputs.h"
#include "Watch.h"
#include "Shader.h"
#include "Model.h"
#include "GameObjectManager.h"
#include <iostream>

//to remove when resource manager
ShaderProgram shaderProg;
GLint viewProjLocation;
GLint ModelLocation;
GLint viewPosLocation;

Application::Application()
{
    InitOpenGl();
    window.Init();
    editorCamera.SetRatio(1920.0f / 1080.0f);
    InitGlad();
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
    //to remove =====================================================
    VertexShader vertex("Resources/Shaders/vert.vert");
    FragmentShader fragment("Resources/Shaders/frag.frag");

    shaderProg.Link(vertex, fragment);

    viewProjLocation = glGetUniformLocation(shaderProg.GetId(), "viewProj");
    ModelLocation = glGetUniformLocation(shaderProg.GetId(), "model");
    viewPosLocation = glGetUniformLocation(shaderProg.GetId(), "viewPos");

    GameObject* base = new GameObject();
    Model* model = new Model(base, "Resources/Models/Car.fbx");
    base->transform.SetScale(lm::FVec3(0.01f));
    base->AddComponent(model);
    testComponent* test = new testComponent(base);
    base->AddComponent(test);
    GameObjectManager::AddGameObject(base);
    //==================================================================

    glEnable(GL_DEPTH_TEST);

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
    //clear
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (isEditor)
    {
        editorCamera.Update();

        lm::FMat4 viewProj = editorCamera.GetProjectionMatrix() * editorCamera.CreateViewMatrix();
        lm::FVec3 viewPos = editorCamera.transform.GetPosition();

        shaderProg.Use();

        float m[16];
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m[(i * 4) + j] = viewProj[i][j];
            }
        }

        float m2[16];

        glUniformMatrix4fv(viewProjLocation, 1, GL_FALSE, m);
        glUniform3f(viewPosLocation, viewPos.x, viewPos.y, viewPos.z);

        for (int i = 0; i < GameObjectManager::GetSize(); i++)
        {
            GameObject* object = GameObjectManager::GetGameObject(i);

            if (object)
            {
                for (int k = 0; k < 4; k++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        m[(k * 4) + j] = viewProj[k][j];
                    }
                }

                glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, m2);
                object->Update();
            }
        }
    }
}