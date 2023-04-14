#include "MenuBarDisplay.h"
#include "InterfaceManager.h"
#include "imgui.h"

MenuBarDisplay::MenuBarDisplay()
{
	InterfaceManager::AddEditorElement(this);
}

MenuBarDisplay::~MenuBarDisplay()
{
}

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
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		ImGui::Separator();
		if (ImGui::MenuItem("Cut", "CTRL+X")) {}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}