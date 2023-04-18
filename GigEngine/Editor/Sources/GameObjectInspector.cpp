#include "GameObjectInspector.h"
#include "InterfaceManager.h"
#include "FileDisplay.h"
#include "imgui.h"
#include "Application.h"
#include "GameObject.h"
#include "GameObjectManager.h"

GameObjectInspector::GameObjectInspector()
{
    InterfaceManager::AddEditorElement(this);
}

GameObjectInspector::~GameObjectInspector()
= default;

void GameObjectInspector::Draw()
{
    height = InterfaceManager::GetHeight() - InterfaceManager::GetClassHeight<FileDisplay>() - g_menuBarSize;
    bool open = true;
    ImGui::SetNextWindowPos({ InterfaceManager::GetWidth() - width, g_menuBarSize });
    ImGui::SetNextWindowSize({ width, height });

    // noMove et NoCollapse
    ImGui::Begin("Inspector", &open, 4 | 32);

    LimitWidthResize();
    ImGui::SetWindowSize("Inspector", { width, height });

    DrawGameObject();

    ImGui::End();
}

void GameObjectInspector::DrawGameObject()
{
    GameObject* object = GameObjectManager::GetFocusedGameObject();
    if (!object) return;

    ImGui::Text(object->GetName().c_str());

    ImGui::Separator();

    DrawTransform(object);
}

void GameObjectInspector::DrawTransform(GameObject* pObject)
{
    ImGui::Text("Transform :");

    lm::FVec3 rot = pObject->GetTransform().GetWorldRotation();
    lm::FVec3 pos = pObject->GetTransform().GetWorldPosition();
    lm::FVec3 scl = pObject->GetTransform().GetWorldScale();

    float translation[] = { pos.x, pos.y, pos.z };
    if (ImGui::SliderFloat3("Position", translation, -100, 100, "%.2f"))
    {
        pObject->GetTransform().SetWorldPosition(lm::FVec3(translation[0], translation[1], translation[2]));
    }

    float scale[] = { scl.x, scl.y, scl.z };
    if (ImGui::SliderFloat3("Scale", scale, -100, 100, "%.2f"))
    {
        pObject->GetTransform().SetWorldScale(lm::FVec3(scale[0], scale[1], scale[2]));
    }

    float rotation[] = { rot.x, rot.y, rot.z };
    if (ImGui::SliderFloat3("Rotation", rotation, -360.0f, 360.0f, "%.2f"))
    {
        pObject->GetTransform().SetWorldRotation(lm::FVec3(rotation[0], rotation[1], rotation[2]));
    }
}