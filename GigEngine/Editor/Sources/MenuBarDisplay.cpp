#include "MenuBarDisplay.h"
#include "InterfaceManager.h"
#include "imgui.h"
#include "imgui_internal.h"
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
		if (ImGui::MenuItem("test2", "CTRL+X")) {}

		ImGui::EndMenu();
	}
	if (ImGui::Button(ICON_SAVE)) 
	{
		//save
	}

	DrawPlayPause();

	ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 12);
	ImGui::PushStyleColor(ImGuiCol_Button, {1,0,0,0.8f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0,0,1 });
	if (ImGui::Button(ICON_CLOSE))
	{
		Application::GetWindow().Close();
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::EndMainMenuBar();
}

void MenuBarDisplay::DrawPlayPause()
{
	ImGui::SameLine(ImGui::GetWindowContentRegionWidth() / 2 - 75);

	const bool isPause = Application::IsInPause();
	const bool isPlaying = !Application::IsInEditor();
	const bool isUsingEditorCam = Application::IsUsingEditorCam();

	if (isPlaying)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0,1,0,0.5f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,1,0,0.6f });
	}

	if (ImGui::Button(ICON_PLAY, { 50, 0 }))
	{
		Application::Play();
	}

	if (isPlaying)
	{
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	if (isPause)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0,1,0,0.5f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,1,0,0.6f });
	}

	if (ImGui::Button(ICON_PAUSE, { 50, 0 }))
	{
		Application::Pause();
	}

	if (isPause)
	{
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	if (isUsingEditorCam)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0,1,0,0.5f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,1,0,0.6f });
	}

	if (ImGui::Button(ICON_CAMERA, { 50, 0 }))
	{
		Application::UseEditorCam();
	}

	if (isUsingEditorCam)
	{
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
}