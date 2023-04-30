#pragma once
#include <functional>
#include "CustomMotionState.h"
#include "GameObjectManager.h"

struct Collision;
enum class RBState
{
    DYNAMIC,
    KINETIC,
    STATIC
};

enum class RigidBodyType
{
    BOX,
    CAPSULE,
    SPHERE
};

using CollisionEnterCallBack = std::function<void(Collision)>;
using CollisionExitCallBack = std::function<void(Collision)>;

struct CollisionCallBacks
{
    CollisionEnterCallBack onEnter;
    CollisionExitCallBack onExit;
};

class RigidBody
{
protected:
    btTransform transform;
    GameObject* owner;
    btRigidBody* body = nullptr;
	RBState state = RBState::DYNAMIC;
    CustomMotionState* motionState = nullptr;
    btCollisionShape* rbShape = nullptr;
    CollisionCallBacks* collisionCallBacks = nullptr;
    RigidBodyType rbType{};
    btScalar mass{};
    lm::FVec3 scale;
    bool gravity;

public:

    RigidBody(GameObject* pOwner);
    ~RigidBody();

    void SetRBState(const RBState& pState) const;
    void SetMotionState(CustomMotionState* pMotionState);

    void SetRigidBodyPosition(const lm::FVec3& pNewPosition);
    void SetRigidBodyRotation(const lm::FVec3& pNewRotation);
    void SetRigidBodyRotation(const lm::FQuat& pNewRotation);
    void SetRigidBodyScale(const lm::FVec3& pNewScale) const;

    void AddRigidBodyPosition(const lm::FVec3& pAddedPosition);
    void AddRigidBodyRotation(const lm::FVec3& pAddedRotation);
    void AddRigidBodyRotation(const lm::FQuat& pAddedRotation);
    void AddRigidBodyScale(const lm::FVec3& pAddedScale) const;

    void AddForce(const float pValue);
    void AddForce(const lm::FVec3& pValue);
    void AddTorque(const float pValue);
    void AddTorque(const lm::FVec3& pValue);
    void SetVelocity(const lm::FVec3& pValue);
    void SetAngularVelocity(const lm::FVec3& pValue);
    float GetFriction();
    void SetFriction(const float pValue);
    void SetBounciness(const float pValue);
    void SetLinearVelocity(const lm::FVec3& pValue);
    lm::FVec3 GetLinearVelocity();
    float GetBounciness();
    lm::FVec3 GetAngularVelocity();
    void SetLinearFactor(const float pValue);
    void SetLinearFactor(const lm::FVec3& pValue);
    void SetAngularFactor(const float pValue);
    lm::FVec3 GetLinearFactor();
    lm::FVec3 GetAngularFactor();
    bool IsTrigger();
    void SetTrigger(const bool pIsTrigger);
    void AddImpulse(const float pValue);
    void AddImpulse(const lm::FVec3& pValue);

    [[nodiscard]] bool IsGravityEnabled() const;
    [[nodiscard]] btRigidBody* GetRigidBody() const;
    [[nodiscard]] CustomMotionState* GetMotionState() const;
    [[nodiscard]] btCollisionShape* GetRBShape() const;
    [[nodiscard]] CollisionCallBacks* GetCallBacks() const;
    RigidBodyType& GetShapeType();
    btScalar& GetMass();
    const lm::FVec3& GetScale();
    void SetMass(const float pValue);
    void SetScale(const lm::FVec3& pNewScale);
    void SetGravityEnabled(const bool pState);
    void ClearForces();
};