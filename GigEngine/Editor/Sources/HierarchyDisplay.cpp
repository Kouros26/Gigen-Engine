#include "HierarchyDisplay.h"
#include "imgui.h"
#include "GameObjectManager.h"
#include "Application.h"
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

	bool treeNodeOpen = ImGui::TreeNode(obj->GetName().c_str());

	ImGui::PushID(obj->GetId());
	ImGui::PopID();

	if (ImGui::IsItemClicked(0))
	{
		GameObjectManager::SetFocusedGameObject(obj);
	}
	if (ImGui::IsItemClicked(1))
	{
		ImGui::OpenPopup(obj->GetName().c_str());
	}
	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
	{
		EditorCamera& cam = Application::GetEditorCamera();
		cam.GetTransform().SetWorldPosition(obj->GetTransform().GetWorldPosition() - cam.GetFront() * 3);
	}

	if (ImGui::BeginPopup(obj->GetName().c_str()))
	{
		ImGui::SeparatorText(obj->GetName().c_str());
		if (ImGui::Button("Destroy"))
		{
			obj->Destroy();
		}
		if (ImGui::Button("UnParent"))
		{
			if (obj->GetParent())
			{
				obj->GetParent()->RemoveChild(obj);
			}
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginDragDropSource())
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
			if (drop != obj && drop && obj && !obj->IsAParent(drop))
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

	if (treeNodeOpen)
	{
		for (int i = 0; i < obj->GetChildrenCount(); i++)
		{
			DisplayGameObject(obj->GetChild(i), treeNodeOpen);
		}
		ImGui::TreePop();
	}
}