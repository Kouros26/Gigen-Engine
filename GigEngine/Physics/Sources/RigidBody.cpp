#include "RigidBody.h"
#include "WorldPhysics.h"

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
}

void RigidBody::SetRigidBodyRotation(const lm::FVec3& pNewRotation)
{
	const btQuaternion orientation(pNewRotation.x, pNewRotation.y, pNewRotation.z);

	transform.setRotation(orientation);
}

void RigidBody::SetRigidBodyRotation(const lm::FQuat& pNewRotation)
{
	const btQuaternion orientation(pNewRotation.x, pNewRotation.y, pNewRotation.z, pNewRotation.w);

	transform.setRotation(orientation);
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
}

void RigidBody::AddRigidBodyRotation(const lm::FVec3& pAddedRotation)
{
	const btQuaternion orientation(pAddedRotation.x, pAddedRotation.y, pAddedRotation.z);

	transform.setRotation(transform.getRotation() + orientation);
}

void RigidBody::AddRigidBodyRotation(const lm::FQuat& pAddedRotation)
{
	const btQuaternion orientation(pAddedRotation.x, pAddedRotation.y, pAddedRotation.z, pAddedRotation.w);

	transform.setRotation(transform.getRotation() + orientation);
}

void RigidBody::AddRigidBodyScale(const lm::FVec3& pAddedScale) const
{
	const btVector3 scale{ pAddedScale.x, pAddedScale.y, pAddedScale.z };

	rbShape->setLocalScaling(rbShape->getLocalScaling() + scale);
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

void RigidBody::SetScale(const lm::FVec3& pNewScale)
{
	scale = pNewScale;
	rbShape->setLocalScaling({ pNewScale.x, pNewScale.y, pNewScale.z });
}