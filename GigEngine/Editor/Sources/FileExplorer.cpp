#include "FileExplorer.h"
#include "imgui.h"

#include <stdio.h>
#include <filesystem>
#include <iostream>

FileExplorer::FileExplorer()
{
	currentDirPath = rootDirPath;
}

FileExplorer::~FileExplorer()
{
}

void FileExplorer::Draw()
{
	width = ImGui::GetWindowContentRegionWidth();
	int columnCount = static_cast<int>(width / (cellSize + padding));
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, nullptr, false);

	if (currentDirPath != rootDirPath)
	{
		ImGui::SetWindowFontScale(3);
		ImGui::Button(ICON_BACK, { cellSize, cellSize });
		ImGui::SetWindowFontScale(1);
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			const int pos = currentDirPath.find_last_of('/');
			currentDirPath = currentDirPath.substr(0, pos);
		}
		ImGui::NextColumn();
	}

	for (auto& directoryEntry : std::filesystem::directory_iterator(currentDirPath))
	{
		const auto fullPath = directoryEntry.path();
		auto relativePath = std::filesystem::relative(fullPath, currentDirPath);
		std::string filename = relativePath.filename().string();

		const char* icon;
		if (directoryEntry.is_directory())
		{
			icon = ICON_MD_FOLDER;
		}
		else
		{
			icon = GetIconOfFile(filename);
		}

		ImGui::SetWindowFontScale(3);
		ImGui::Button(icon, { cellSize, cellSize });
		ImGui::SetWindowFontScale(1);

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && directoryEntry.is_directory())
		{
			currentDirPath += "/" + filename;
		}

		if (!directoryEntry.is_directory())
		{
			if (ImGui::BeginDragDropSource())
			{
				std::string itemPath = fullPath.string();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath.c_str(), sizeof(wchar_t) * (itemPath.length() + 1));
				ImGui::EndDragDropSource();
			}
		}

		ImGui::TextWrapped(filename.c_str());
		ImGui::NextColumn();
	}

	ImGui::Columns(1);
}

const char* FileExplorer::GetIconOfFile(const std::string& str) const
{
	if (str.find(".obj") != std::string::npos
		|| str.find(".OBJ") != std::string::npos
		|| str.find(".fbx") != std::string::npos
		|| str.find(".FBX") != std::string::npos)
	{
		return ICON_MODEL;
	}
	if (str.find(".png") != std::string::npos ||
		str.find(".jpg") != std::string::npos ||
		str.find(".jpeg") != std::string::npos)
	{
		return ICON_TEXTURE;
	}
	if (str.find(".lua") != std::string::npos)
	{
		return ICON_COMPONENT;
	}

	return ICON_MD_DESCRIPTION;
}