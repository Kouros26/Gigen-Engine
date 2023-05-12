#include "ToolsDisplay.h"
#include "HierarchyDisplay.h"
#include "InterfaceManager.h"
#include "Console.h"
#include "imgui.h"

#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <stdlib.h>

ToolsDisplay::ToolsDisplay()
{
    InterfaceManager::AddEditorElement(this);
    InterfaceManager::AddEditorElement(this);
    currentDirPath = rootDirPath;
}

ToolsDisplay::~ToolsDisplay()
{
}

void ToolsDisplay::Draw()
{
    width = InterfaceManager::GetWidth() - InterfaceManager::GetClassWidth<HierarchyDisplay>();
    ImGui::SetNextWindowPos({ InterfaceManager::GetWidth() - width,  InterfaceManager::GetHeight() - height });
    ImGui::SetNextWindowSize({ width, height });

    //noMove et NoCollapse
    ImGui::Begin("Tools", NULL, 4 | 32);

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
        if (ImGui::BeginTabItem("Profiler"))
        {
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
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
    DrawFiles(currentDirPath);
}

void ToolsDisplay::DrawFiles(const std::string& path)
{
    int columnCount = (int)(width / (cellSize + padding));
    if (columnCount < 1)
        columnCount = 1;

    ImGui::Columns(columnCount, 0, false);

    if (path != rootDirPath)
    {
        ImGui::Button("..", { cellSize, cellSize });
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            int pos = currentDirPath.find_last_of("/");
            currentDirPath = currentDirPath.substr(0, pos);
        }
        ImGui::NextColumn();
    }

    for (auto& directoryEntry : std::filesystem::directory_iterator(path))
    {
        const auto fullPath = directoryEntry.path();
        auto relativePath = std::filesystem::relative(fullPath, path);
        std::string filename = relativePath.filename().string();

        ImGui::Button(filename.c_str(), { cellSize, cellSize });

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && !directoryEntry.is_directory())
        {
            std::string itemPath = "start " + fullPath.string();
            system(itemPath.c_str());
        }

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

void ToolsDisplay::DrawConsole()
{
    Console::Draw();
}