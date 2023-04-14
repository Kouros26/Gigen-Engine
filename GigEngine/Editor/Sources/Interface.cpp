#include "Interface.h"
#include "InterfaceManager.h"
#include <imgui_impl_opengl3.h>
#include "imgui_impl_glfw.h"

Interface::Interface(GLFWwindow* pWindow, const char* pGlslVersion)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init(pGlslVersion);
    ImGui::StyleColorsDark();
}

Interface::~Interface()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Interface::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Interface::Draw()
{
    InterfaceManager::DrawEditor();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}