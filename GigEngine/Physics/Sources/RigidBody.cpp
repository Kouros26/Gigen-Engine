#include "RigidBody.h"
#include "WorldPhysics.h"
#include "Utilities.h"
#include "Mat3/FMat3.hpp"

RigidBody::RigidBody(GameObject* pOwner)
	: owner(pOwner) {}

RigidBody::~RigidBody()
{
	WorldPhysics::RemoveRigidBodyFromWorld(body);
	delete body;
}

void RigidBody::SetRBState(const RBState& pState) const
{
	switch (pState)
	{
	case RBState::DYNAMIC:
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_DYNAMIC_OBJECT);
		break;
	case RBState::KINETIC:
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		break;
	case RBState::STATIC:
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		break;
	}
}

void RigidBody::SetMotionState(CustomMotionState* pMotionState)
{
	motionState = pMotionState;
}

void RigidBody::SetRigidBodyPosition(const lm::FVec3& pNewPos)
{
	const btVector3 position(pNewPos.x, pNewPos.y, pNewPos.z);

	transform.setOrigin(position);
	body->setWorldTransform(transform);
	body->activate(true);
}

void RigidBody::SetRigidBodyRotation(const lm::FVec3& pNewRotation)
{
	const btQuaternion orientation(lm::degreesToRadians(pNewRotation.y), lm::degreesToRadians(pNewRotation.x), lm::degreesToRadians(pNewRotation.z));

	transform.setRotation(orientation);
	body->setWorldTransform(transform);
}

void RigidBody::SetRigidBodyRotation(const lm::FQuat& pNewRotation)
{
	const btQuaternion orientation(pNewRotation.x, pNewRotation.y, pNewRotation.z, pNewRotation.w);

	transform.setRotation(orientation);
	body->setWorldTransform(transform);
}

void RigidBody::SetRigidBodyScale(const lm::FVec3& pNewScale) const
{
	const btVector3 scale{ pNewScale.x, pNewScale.y, pNewScale.z };

	rbShape->setLocalScaling(scale);
}

void RigidBody::AddRigidBodyPosition(const lm::FVec3& pAddedPosition)
{
	const btVector3 position(pAddedPosition.x, pAddedPosition.y, pAddedPosition.z);

	transform.setOrigin(transform.getOrigin() + position);
	body->setWorldTransform(transform);
	body->activate(true);
}

void RigidBody::AddRigidBodyRotation(const lm::FVec3& pAddedRotation)
{
	const btQuaternion orientation(lm::degreesToRadians(pAddedRotation.y), lm::degreesToRadians(pAddedRotation.x), lm::degreesToRadians(pAddedRotation.z));

	transform.setRotation(transform.getRotation() * orientation);
	body->setWorldTransform(transform);
}

void RigidBody::AddRigidBodyRotation(const lm::FQuat& pAddedRotation)
{
	const btQuaternion orientation(pAddedRotation.x, pAddedRotation.y, pAddedRotation.z, pAddedRotation.w);

	transform.setRotation(transform.getRotation() + orientation);
	body->setWorldTransform(transform);
}

void RigidBody::AddRigidBodyScale(const lm::FVec3& pAddedScale) const
{
	const btVector3 scale{ pAddedScale.x, pAddedScale.y, pAddedScale.z };

	rbShape->setLocalScaling(rbShape->getLocalScaling() + scale);
}

void RigidBody::AddForce(const float pValue)
{
	body->applyForce(btVector3(pValue, pValue, pValue), btVector3(0, 0, 0));
}

void RigidBody::AddForce(const lm::FVec3& pValue)
{
	body->applyForce(btVector3(pValue.x, pValue.y, pValue.z), btVector3(0, 0, 0));
}

void RigidBody::AddTorque(const float pValue)
{
	body->applyTorque(btVector3(pValue, pValue, pValue));
}

void RigidBody::AddTorque(const lm::FVec3& pValue)
{
	body->applyTorque(btVector3(pValue.x, pValue.y, pValue.z));
}

void RigidBody::SetVelocity(const lm::FVec3& pValue)
{
	body->setLinearVelocity(btVector3(pValue.x, pValue.y, pValue.z));
}

lm::FVec3 RigidBody::GetVelocity() const
{
	btVector3 velocity = body->getLinearVelocity();
	return lm::FVec3(velocity.x(), velocity.y(), velocity.z());
}

void RigidBody::SetAngularVelocity(const lm::FVec3& pValue)
{
	body->setAngularVelocity(btVector3(pValue.x, pValue.y, pValue.z));
}

float RigidBody::GetFriction()
{
	return body->getFriction();
}

void RigidBody::SetFriction(const float pValue)
{
	body->setFriction(pValue);
}

void RigidBody::SetBounciness(const float pValue)
{
	body->setRestitution(pValue);
}

void RigidBody::SetLinearVelocity(const lm::FVec3& pValue)
{
	body->setLinearVelocity(btVector3(pValue.x, pValue.y, pValue.z));
}

lm::FVec3 RigidBody::GetLinearVelocity() const
{
	btVector3 linearVelocity = body->getLinearVelocity();
	return lm::FVec3(linearVelocity.x(), linearVelocity.y(), linearVelocity.z());
}

float RigidBody::GetBounciness()
{
	return body->getRestitution();
}

lm::FVec3 RigidBody::GetAngularVelocity() const
{
	btVector3 angularVelocity = body->getAngularVelocity();
	return lm::FVec3(angularVelocity.x(), angularVelocity.y(), angularVelocity.z());
}

void RigidBody::SetLinearFactor(const float pValue)
{
	body->setAngularFactor(btVector3(pValue, pValue, pValue));
}

void RigidBody::SetLinearFactor(const lm::FVec3& pValue)
{
	body->setAngularFactor(btVector3(pValue.x, pValue.y, pValue.z));
}

void RigidBody::SetAngularFactor(const float pValue)
{
	body->setAngularFactor(pValue);
}

lm::FVec3 RigidBody::GetLinearFactor() const
{
	return lm::FVec3(body->getAngularFactor().x(), body->getAngularFactor().y(), body->getAngularFactor().z());
}

lm::FVec3 RigidBody::GetAngularFactor() const
{
	btVector3 angularFactor = body->getAngularFactor();
	return lm::FVec3(angularFactor.x(), angularFactor.y(), angularFactor.z());
}

bool RigidBody::IsTrigger()
{
	return false;
}

void RigidBody::SetTrigger(const bool pIsTrigger)
{
}

void RigidBody::AddImpulse(const float pValue)
{
	body->applyCentralImpulse(btVector3(pValue, pValue, pValue));
}

void RigidBody::AddImpulse(const lm::FVec3& pValue)
{
	body->applyCentralImpulse(btVector3(pValue.x, pValue.y, pValue.z));
}

btRigidBody* RigidBody::GetRigidBody() const
{
	return body;
}

CustomMotionState* RigidBody::GetMotionState() const
{
	return motionState;
}

btCollisionShape* RigidBody::GetRBShape() const
{
	return rbShape;
}

CollisionCallBacks* RigidBody::GetCallBacks() const
{
	return collisionCallBacks;
}

RigidBodyType& RigidBody::GetShapeType()
{
	return rbType;
}

btScalar& RigidBody::GetMass()
{
	return mass;
}

const lm::FVec3& RigidBody::GetScale()
{
	return scale;
}

void RigidBody::SetMass(const float pValue)
{
	mass = pValue;
}

void RigidBody::SetScale(const lm::FVec3& pNewScale)
{
	scale = pNewScale;
	rbShape->setLocalScaling({ pNewScale.x, pNewScale.y, pNewScale.z });
}

void RigidBody::SetGravityEnabled(const bool pState) const
{
	if (!pState)
		body->setGravity({ 0,0,0, });

	else
		body->setGravity({ 0,-9.81f,0, });
}

void RigidBody::ClearForces()
{
	body->clearForces();
}

void RigidBody::SetGravity(const lm::FVec3& pValue) const
{
	body->setGravity({ pValue.x, pValue.y, pValue.z });
}

lm::FVec3 RigidBody::GetGravity() const
{
	btVector3 gravity = body->getGravity();
	return lm::FVec3(gravity.x(), gravity.y(), gravity.z());
}
