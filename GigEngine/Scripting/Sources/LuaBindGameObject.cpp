#include "LuaBindGameObject.h"
#include "GameObject.h"
#include "Behaviour.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Model.h"
#include "Texture.h"
#include "Component.h"

void GigScripting::LuaBindGameObject::BindGameObject(sol::state& pLuaState)
{
    auto& luaState = pLuaState;

    luaState.new_usertype<GameObject>("GameObject",

        //"AddComponent", sol::resolve<void(Component*)>(&GameObject::AddComponent),

        //"GetComponent", &GameObject::GetComponent,
        "GetTransform", &GameObject::GetTransform,
        "GetRigidBody", &GameObject::GetRigidBody,
        "GetName", &GameObject::GetName,
        "SetName", &GameObject::SetName,
        "GetID", &GameObject::GetId,
        "GetParent", &GameObject::GetParent,
        "SetParent", &GameObject::SetParent,
        "GetChildren", &GameObject::GetChildren,

        "AddChild", &GameObject::AddChild,
        "RemoveChild", &GameObject::RemoveChild,
        "GetChild", &GameObject::GetChild,
        "SetActive", &GameObject::SetActive,
        "IsActive", &GameObject::IsActive,
        "GetChildrenCount", &GameObject::GetChildrenCount,

        "GetRigidBody", &GameObject::GetRigidBody,
        "GetModel", &GameObject::GetModel,
        "GetTexture", &GameObject::GetTexture,
        "SetModel", &GameObject::SetModelWithPathLua,
        "SetTexture", &GameObject::SetTextureWithPathLua,

        "GetBehaviour", [](GameObject& p_this, const std::string& pName) -> sol::table
        {
            auto behaviour = p_this.GetBehaviour(pName);
            if (behaviour)
                return behaviour->GetScriptTable();
            else
                return sol::nil;
        }

    );
}