#include "HierarchyDisplay.h"
#include "imgui.h"
#include "GameObjectManager.h"
#include "GameObject.h"
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
	ImGui::ShowDemoWindow();

	height = InterfaceManager::GetHeight();
	bool open = true;
	ImGui::SetNextWindowPos({ 0, g_menuBarSize });
	ImGui::SetNextWindowSize({ width, height });
	//noMove et NoCollapse
	ImGui::Begin("Scene", &open, 4 | 32);

	LimitWidthResize();
	ImGui::SetWindowSize("Scene", { width, height });

	node_clicked = -1;
	DisplayHierarchy();

	ImGui::End();
}

void HierarchyDisplay::DisplayHierarchy()
{
	for (int i = 0; i < GameObjectManager::GetSize(); i++) 
	{
		DisplayGameObject(GameObjectManager::GetGameObject(i), false);
	}
}

void HierarchyDisplay::DisplayGameObject(GameObject* obj, bool isChild)
{
	if (obj->GetParent() && !isChild)
	{
		return;
	}

	ImGui::PushID(obj->GetId());
	bool treeNodeOpen = ImGui::TreeNode(obj->GetName().c_str());

	for (int i = 0; i < obj->GetChildrenCount(); i++)
	{
		DisplayGameObject(obj->GetChild(i), treeNodeOpen);
	}
	if (ImGui::IsItemClicked(0)) 
	{
		if (node_clicked == -1) 
		{
			node_clicked = obj->GetId();
			GameObjectManager::SetFocusedGameObject(obj);
		}
	}
	
	if(ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("HIERARCHY", (const void*)obj, sizeof(const void*));
		ImGui::Text(obj->GetName().c_str());
		ImGui::EndDragDropSource();
	}
	
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* truc = ImGui::AcceptDragDropPayload("HIERARCHY"))
		{
			GameObject* drop = (GameObject*)truc->Data;
			if (drop != obj && drop && obj)
			{
				if (drop->GetParent()) 
				{
					drop->GetParent()->RemoveChild(drop);
				}
				obj->AddChild(drop);
			}
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::PopID();

	if (treeNodeOpen) 
	{
		ImGui::TreePop();
	}
}
