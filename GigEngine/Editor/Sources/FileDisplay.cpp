#include "FileDisplay.h"
#include "HierarchyDisplay.h"
#include "InterfaceManager.h"
#include "imgui.h"
#include <stdio.h>
#include <filesystem>
#include <iostream>

FileDisplay::FileDisplay()
{
	InterfaceManager::AddEditorElement(this);
}

FileDisplay::~FileDisplay()
= default;

void FileDisplay::Draw()
{
	width = InterfaceManager::GetWidth() - InterfaceManager::GetClassWidth<HierarchyDisplay>();
	ImGui::SetNextWindowPos({ InterfaceManager::GetWidth() - width,  InterfaceManager::GetHeight() - height });
	ImGui::SetNextWindowSize({ width, height });
	//noMove et NoCollapse
	ImGui::Begin("Files", nullptr, 4 | 32);

	LimitHeightResize();
	ImGui::SetWindowSize("Files", { width, height });

	DrawFolders("Resources/");

	ImGui::End();
}

void FileDisplay::DrawFolders(std::string path)
{
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	for (auto& p : std::filesystem::recursive_directory_iterator(path))
		if (p.is_directory())
			std::cout << p.path() << std::endl;
}