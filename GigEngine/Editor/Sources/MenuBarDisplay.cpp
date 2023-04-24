#include "MenuBarDisplay.h"
#include "InterfaceManager.h"
#include "imgui.h"
#include "Application.h"

MenuBarDisplay::MenuBarDisplay()
{
	InterfaceManager::AddEditorElement(this);
}

MenuBarDisplay::~MenuBarDisplay()
= default;

void MenuBarDisplay::Draw()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("test", "shortcut", false, true)) {}
		if (ImGui::MenuItem("test2", "truc", false, false)) {}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("test", "CTRL+Z")) {}
		ImGui::Separator();
		if (ImGui::MenuItem("test2", "CTRL+X")) {}

		ImGui::EndMenu();
	}

	if (ImGui::Button("Close"))
	{
		Application::GetWindow().Close();
	}

	ImGui::EndMainMenuBar();
}