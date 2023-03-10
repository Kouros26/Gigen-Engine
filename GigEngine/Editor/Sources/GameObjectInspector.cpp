#include "GameObjectInspector.h"
#include "InterfaceManager.h"
#include "imgui.h"

GameObjectInspector::GameObjectInspector()
{
	InterfaceManager::AddEditorElement(this);
}

GameObjectInspector::~GameObjectInspector()
= default;

void GameObjectInspector::Draw()
{
	ImGui::Begin("Scene");

	GetGameObjects();

	ImGui::End();
}

void GameObjectInspector::GetGameObjects()
{

	GameObject& object = *GameObjectManager::GetGameObject(1);

	ImGui::Text(object.GetName().c_str());

	static bool t = false;
	ImGui::Checkbox("ScaleUp", &t);

	if (t)
		object.GetTransform().AddScale({ 0.02f });
}
