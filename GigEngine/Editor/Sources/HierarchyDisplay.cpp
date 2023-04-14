#include "HierarchyDisplay.h"
#include "imgui.h"
#include "InterfaceManager.h"

HierarchyDisplay::HierarchyDisplay()
{
	InterfaceManager::AddEditorElement(this);
}

HierarchyDisplay::~HierarchyDisplay()
{
}

void HierarchyDisplay::Draw()
{
	height = InterfaceManager::GetHeight();
	bool open = true;
	ImGui::SetNextWindowPos({ 0, g_menuBarSize });
	ImGui::SetNextWindowSize({ width, height });
	//noMove et NoCollapse
	ImGui::Begin("Scene", &open, 4 | 32);

	LimitWidthResize();
	ImGui::SetWindowSize("Scene", { width, height });

	ImGui::End();
}