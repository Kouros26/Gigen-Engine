#include "ToolsDisplay.h"
#include "HierarchyDisplay.h"
#include "InterfaceManager.h"
#include "Console.h"
#include "imgui.h"

ToolsDisplay::ToolsDisplay()
{
	InterfaceManager::AddEditorElement(this);
}

ToolsDisplay::~ToolsDisplay()
{
}

void ToolsDisplay::Draw()
{
	ImGui::ShowDemoWindow();

	width = InterfaceManager::GetWidth() - InterfaceManager::GetClassWidth<HierarchyDisplay>();
	bool open = true;
	ImGui::SetNextWindowPos({ InterfaceManager::GetWidth() - width,  InterfaceManager::GetHeight() - height });
	ImGui::SetNextWindowSize({ width, height });
	//noMove et NoCollapse

	ImGui::Begin("Tools", &open, 4 | 32);

	LimitHeightResize();
	ImGui::SetWindowSize("Tools", { width, height });

    if (ImGui::BeginTabBar("MyTabBar"))
    {
        if (ImGui::BeginTabItem("Explorer"))
        {
            DrawExplorer();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Console"))
        {
            DrawConsole();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

	ImGui::End();
}

void ToolsDisplay::DrawExplorer()
{
}

void ToolsDisplay::DrawConsole()
{
    Console::Draw();
}
