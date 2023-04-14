#include "FileDisplay.h"
#include "HierarchyDisplay.h"
#include "InterfaceManager.h"
#include "imgui.h"

FileDisplay::FileDisplay()
{
	InterfaceManager::AddEditorElement(this);
}

FileDisplay::~FileDisplay()
{
}

void FileDisplay::Draw()
{
	width = InterfaceManager::GetWidth() - InterfaceManager::GetClassWidth<HierarchyDisplay>();
	bool open = true;
	ImGui::SetNextWindowPos({ InterfaceManager::GetWidth() - width,  InterfaceManager::GetHeight() - height });
	ImGui::SetNextWindowSize({ width, height });
	//noMove et NoCollapse
	ImGui::Begin("Files", &open, 4 | 32);

	LimitHeightResize();
	ImGui::SetWindowSize("Files", { width, height });

	ImGui::End();
}