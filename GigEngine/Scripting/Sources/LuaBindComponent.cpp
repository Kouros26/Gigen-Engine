#include "LuaBindComponent.h"
#include "Component.h"
#include "Transform.h"
#include "RigidBody.h"
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
        "SetPosition", &Transform::SetLocalPosition,
        "SetRotation", &Transform::SetLocalRotation,
        "SetScale", &Transform::SetLocalScale,
        "GetPosition", &Transform::GetLocalPosition,
        "GetRotation", &Transform::GetLocalRotation,
        "GetScale", &Transform::GetLocalScale,
        "GetWorldPosition", &Transform::GetWorldPosition,
        "GetWorldRotation", &Transform::GetWorldRotation,
        "GetWorldScale", &Transform::GetWorldScale,
        "GetFront", &Transform::GetFront,
        "GetUp", &Transform::GetUp,
        "GetRight", &Transform::GetRight,
        "GetOrientation", &Transform::GetOrientation,
        "SetWorldPosition", &Transform::SetWorldPosition,
        "SetWorldRotation", &Transform::SetWorldRotation,
        "SetWorldScale", &Transform::SetWorldScale,
        "LookAt", &Transform::LookAt

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
            sol::resolve<void(const lm::FVec3&)const >(&RigidBody::AddForce),
            sol::resolve<void(const float)const >(&RigidBody::AddForce)

        ),
        "AddTorque", sol::overload
        (
            sol::resolve<void(const lm::FVec3&)const>(&RigidBody::AddTorque),
            sol::resolve<void(const float)const>(&RigidBody::AddTorque)

        ),
        "SetVelocity", &RigidBody::SetVelocity,
        "GetVelocity", &RigidBody::GetLinearVelocity,
        "SetAngularVelocity", &RigidBody::SetAngularVelocity,
        "GetMass", &RigidBody::GetMass,
        "ClearForces", &RigidBody::ClearForces,
        "GetFriction", &RigidBody::GetFriction,
        "SetFriction", &RigidBody::SetFriction,
        "GetBounciness", &RigidBody::GetBounciness,
        "SetBounciness", &RigidBody::SetBounciness,
        "GetAngularVelocity", &RigidBody::GetAngularVelocity,
        "SetLinearFactor", sol::overload
        (

            sol::resolve<void(const lm::FVec3&)const>(&RigidBody::SetLinearFactor),
            sol::resolve<void(const float)const>(&RigidBody::SetLinearFactor)
        ),
        "SetAngularFactor", &RigidBody::SetAngularFactor,
        "GetLinearFactor", &RigidBody::GetLinearFactor,
        "GetAngularFactor", &RigidBody::GetAngularFactor,
        "IsTrigger", &RigidBody::IsTrigger,
        "SetTrigger", &RigidBody::SetTrigger,
        "AddImpulse", sol::overload
        (
            sol::resolve<void(const lm::FVec3&)const>(&RigidBody::AddImpulse),
            sol::resolve<void(const float)const>(&RigidBody::AddImpulse)
        ),
        "SetGravity", &RigidBody::SetGravity,
        "SetGravityEnabled", &RigidBody::SetGravityEnabled,
        "GetGravity", &RigidBody::GetGravity

    );
}