#include "LuaBindComponent.h"
#include "Component.h"
#include "Transform.h"
#include "Model.h"
#include "RigidBody.h"
#include "Texture.h"
#include "GameObject.h"

void GigScripting::LuaBindComponent::BindComponent(sol::state& pLuaState)
{
    auto& luaState = pLuaState;

    luaState.new_usertype<Component>("Component",
        "GetOwner", [](Component* pComp) { return pComp->GetGameObject(); }
    );

    luaState.new_usertype<Transform>("Transform",
        "AddPosition", &Transform::AddPosition,
        "AddRotation", &Transform::AddRotation,
        "AddScale", &Transform::AddScale,
        "SetLocalPosition", &Transform::SetLocalPosition,
        "SetLocalRotation", &Transform::SetLocalRotation,
        "SetLocalScale", &Transform::SetLocalScale,
        "GetLocalPosition", &Transform::GetLocalPosition,
        "GetLocalRotation", &Transform::GetLocalRotation,
        "GetLocalScale", &Transform::GetLocalScale,
        "GetWorldPosition", &Transform::GetWorldPosition,
        "GetWorldRotation", &Transform::GetWorldRotation,
        "GetWorldScale", &Transform::GetWorldScale,
        "GetFront", &Transform::GetFront,
        "GetUp", &Transform::GetUp,
        "GetRight", &Transform::GetRight,
        "GetOrientation", &Transform::GetOrientation,
        "SetWorldPosition", &Transform::SetWorldPosition,
        "SetWorldRotation", &Transform::SetWorldRotation,
        "SetWorldScale", &Transform::SetWorldScale

    );

    luaState.new_enum<RigidBodyType>("RbShapeType",
        {
            {"BOX", RigidBodyType::BOX},
            {"SPHERE", RigidBodyType::SPHERE},
            {"CAPSULE", RigidBodyType::CAPSULE}
        }
    );

    luaState.new_usertype<RigidBody>("RigidBody",
        "SetMass", &RigidBody::SetMass,
        "AddForce", sol::overload
        (
            sol::resolve<void(const lm::FVec3&)>(&RigidBody::AddForce),
            sol::resolve<void(const float)>(&RigidBody::AddForce)

        ),
        "AddTorque", sol::overload
        (
            sol::resolve<void(const lm::FVec3&)>(&RigidBody::AddTorque),
            sol::resolve<void(const float)>(&RigidBody::AddTorque)

        ),
        "SetVelocity", &RigidBody::SetVelocity,
        "SetAngularVelocity", &RigidBody::SetAngularVelocity,
        "GetMass", &RigidBody::GetMass,
        "ClearForces", &RigidBody::ClearForces,
        "GetFriction", &RigidBody::GetFriction,
        "SetFriction", &RigidBody::SetFriction,
        "GetBounciness", &RigidBody::GetBounciness,
        "SetBounciness", &RigidBody::SetBounciness,
        "SetLinearVelocity", &RigidBody::SetLinearVelocity,
        "GetLinearVelocity", &RigidBody::GetLinearVelocity,
        "GetAngularVelocity", &RigidBody::GetAngularVelocity,
        "SetLinearFactor", sol::overload
        (

            sol::resolve<void(const lm::FVec3&)>(&RigidBody::SetLinearFactor),
            sol::resolve<void(const float)>(&RigidBody::SetLinearFactor)
        ),
        "SetAngularFactor", &RigidBody::SetAngularFactor,
        "GetLinearFactor", &RigidBody::GetLinearFactor,
        "GetAngularFactor", &RigidBody::GetAngularFactor,
        "IsTrigger", &RigidBody::IsTrigger,
        "SetTrigger", &RigidBody::SetTrigger,
        "AddImpulse", sol::overload
        (
            sol::resolve<void(const lm::FVec3&)>(&RigidBody::AddImpulse),
            sol::resolve<void(const float)>(&RigidBody::AddImpulse)
        )

    );
}