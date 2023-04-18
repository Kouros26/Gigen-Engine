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
    //ok
}

void GameObjectInspector::DrawGameObject()
{
    GameObject* object = GameObjectManager::GetFocusedGameObject();
    if (!object) return;

    static char name[128];
    strcpy(name, object->GetName().c_str());

    ImGui::Text("Name"); ImGui::SameLine();

    ImGui::PushItemWidth(-1);
    if (ImGui::InputText("Name", name, 128)) 
    {
        object->SetName(name);
    }
    ImGui::PopItemWidth();

    ImGui::Separator();

    DrawTransform(object);
}

void GameObjectInspector::DrawTransform(GameObject* pObject)
{
    if (ImGui::CollapsingHeader("Transform")) 
    {
        lm::FVec3 rot = pObject->GetTransform().GetWorldRotation();
        lm::FVec3 pos = pObject->GetTransform().GetWorldPosition();
        lm::FVec3 scl = pObject->GetTransform().GetWorldScale();

        float translation[] = { pos.x, pos.y, pos.z };
        float scale[] = { scl.x, scl.y, scl.z };
        float rotation[] = { rot.x, rot.y, rot.z };

        ImGui::Text("Position"); ImGui::SameLine();

        ImGui::PushItemWidth(-1);
        if (ImGui::DragFloat3("p", translation, g_maxStep, g_floatMin, g_floatMax, g_floatFormat))
        {
            pObject->GetTransform().SetWorldPosition(lm::FVec3(translation[0], translation[1], translation[2]));
        }
        ImGui::PopItemWidth();

        ImGui::Text("Scale"); ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        if (ImGui::DragFloat3("s", scale, g_maxStep, g_floatMin, g_floatMax, g_floatFormat))
        {
            pObject->GetTransform().SetWorldScale(lm::FVec3(scale[0], scale[1], scale[2]));
        }
        ImGui::PopItemWidth();

        ImGui::Text("Rotation"); ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        if (ImGui::DragFloat3("r", rotation, g_maxStep, -360.0f, 360.0f, g_floatFormat))
        {
            pObject->GetTransform().SetWorldRotation(lm::FVec3(rotation[0], rotation[1], rotation[2]));
        }
        ImGui::PopItemWidth();
    }
}