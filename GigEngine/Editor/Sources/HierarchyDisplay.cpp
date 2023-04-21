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
	ImGui::SetNextWindowPos({ 0, g_menuBarSize });
	ImGui::SetNextWindowSize({ width, height });
	//noMove | NoCollapse
	ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	LimitWidthResize();
	ImGui::SetWindowSize("Scene", { width, height });

	CreatePopUp();
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

void HierarchyDisplay::CreatePopUp()
{
	if (ImGui::Button("Create"))
		ImGui::OpenPopup("createPopUp");

	if (ImGui::BeginPopup("createPopUp"))
	{
		ImGui::SeparatorText("Create");
		if (ImGui::MenuItem("Empty"))
		{
			GameObjectManager::CreateGameObject();
		}
		if (ImGui::BeginMenu("Basics"))
		{
			if (ImGui::MenuItem("Arrow"))
			{
				GameObject* obj = GameObjectManager::CreateGameObject("Arrow");
				obj->SetModel("Resources/Models/Basics/Arrow.FBX");
			}
			if (ImGui::MenuItem("Capsule"))
			{
				GameObject* obj = GameObjectManager::CreateGameObject("Capsule");
				obj->SetModel("Resources/Models/Basics/Capsule.FBX");
			}
			if (ImGui::MenuItem("Cone"))
			{
				GameObject* obj = GameObjectManager::CreateGameObject("Cone");
				obj->SetModel("Resources/Models/Basics/Cone.FBX");
			}
			if (ImGui::MenuItem("Cube"))
			{
				GameObject* obj = GameObjectManager::CreateGameObject("Cube");
				obj->SetModel("Resources/Models/Basics/Cube.FBX");
			}
			if (ImGui::MenuItem("Cylinder"))
			{
				GameObject* obj = GameObjectManager::CreateGameObject("Cylinder");
				obj->SetModel("Resources/Models/Basics/Cylinder.FBX");
			}
			if (ImGui::MenuItem("Plane"))
			{
				GameObject* obj = GameObjectManager::CreateGameObject("Plane");
				obj->SetModel("Resources/Models/Basics/Plane.FBX");
			}
			if (ImGui::MenuItem("Sphere"))
			{
				GameObject* obj = GameObjectManager::CreateGameObject("Sphere");
				obj->SetModel("Resources/Models/Basics/Sphere.FBX");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Lights"))
		{
			if (ImGui::MenuItem("Directionnal Light"))
			{
				GameObjectManager::CreateDirLight();
			}
			if (ImGui::MenuItem("PointLight"))
			{
				GameObjectManager::CreatePointLight();
			}
			if (ImGui::MenuItem("SpotLight"))
			{
				GameObjectManager::CreateSpotLight();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Camera"))
		{
			GameObjectManager::CreateCamera();
		}
		ImGui::EndPopup();
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

	GameObjectClicked(obj);
	GameObjectPopUp(obj);

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

void HierarchyDisplay::GameObjectClicked(GameObject* obj)
{
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
}

void HierarchyDisplay::GameObjectPopUp(GameObject* obj)
{
	if (ImGui::BeginPopup(obj->GetName().c_str()))
	{
		ImGui::SeparatorText(obj->GetName().c_str());
		if (ImGui::MenuItem("Destroy"))
		{
			obj->Destroy();
		}
		if (ImGui::MenuItem("UnParent"))
		{
			if (obj->GetParent())
			{
				obj->GetParent()->RemoveChild(obj);
			}
		}
		if (ImGui::MenuItem("Clone"))
		{
			GameObjectManager::CreateGameObject(obj);
		}
		ImGui::EndPopup();
	}
}