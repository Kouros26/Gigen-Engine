#include "Transform.h"

Transform::Transform(const lm::FVec3& pos, const lm::FVec3& rot, const lm::FVec3& scl)
    : worldPosition(pos), worldRotation(rot), worldScale(scl), localPosition(pos), localRotation(rot), localScale(scl)
{
}

Transform::~Transform()
{
}

void Transform::SetWorldPosition(const lm::FVec3& pos)
{
    localPosition = pos - worldPosition + localPosition;
    worldPosition = pos;
    hasChanged = true;
}

void Transform::SetWorldRotation(const lm::FVec3& rot)
{
    localRotation = rot;
    worldRotation = rot;
    LimitRotation();
    hasChanged = true;
}

void Transform::SetWorldScale(const lm::FVec3& scl)
{
    localScale = (scl / worldScale) * localScale;
    worldScale = scl;
    hasChanged = true;
}

void Transform::SetLocalPosition(const lm::FVec3& pos)
{
    worldPosition = worldPosition + (pos - localPosition);
    localPosition = pos;
    hasChanged = true;
}

void Transform::SetLocalRotation(const lm::FVec3& rot)
{
    worldRotation = rot;
    localRotation = rot;
    hasChanged = true;
}

void Transform::SetLocalScale(const lm::FVec3& scl)
{
    worldScale = worldScale * (scl / localScale);
    localScale = scl;
    hasChanged = true;
}

void Transform::AssignWorldPosition(const lm::FVec3& pos)
{
    worldPosition = pos;
    hasChanged = true;
}

void Transform::AssignWorldRotation(const lm::FVec3& rot)
{
    worldRotation = rot;
    hasChanged = true;
}

void Transform::AssignWorldScale(const lm::FVec3& scl)
{
    worldScale = scl;
    hasChanged = true;
}

void Transform::AssignLocalPosition(const lm::FVec3& pos)
{
    localPosition = pos;
    hasChanged = true;
}

void Transform::AssignLocalRotation(const lm::FVec3& rot)
{
    localRotation = rot;
    hasChanged = true;
}

void Transform::AssignLocalScale(const lm::FVec3& scl)
{
    localScale = scl;
    hasChanged = true;
}

lm::FVec3 Transform::GetWorldPosition() const
{
    return worldPosition;
}

lm::FVec3 Transform::GetWorldScale() const
{
    return worldScale;
}

lm::FVec3 Transform::GetWorldRotation() const
{
    return worldRotation;
}

lm::FQuat Transform::GetOrientation()
{
	const lm::FQuat rotX(lm::FVec3{ 1,0,0 }, worldRotation[0]);

	const lm::FQuat rotY(lm::FVec3{ 0,1,0 }, worldRotation[1]);

	const lm::FQuat rotZ(lm::FVec3{ 0,0,1 }, worldRotation[2]);

    return  rotX * rotY * rotZ;
}

lm::FVec3 Transform::GetLocalPosition() const
{
    return localPosition;
}

lm::FVec3 Transform::GetLocalScale() const
{
    return localScale;
}

lm::FVec3 Transform::GetLocalRotation() const
{
    return localRotation;
}

void Transform::AddPosition(const lm::FVec3& pos)
{
    worldPosition += pos;
    localPosition += pos;
    hasChanged = true;
}

void Transform::AddRotation(const lm::FVec3& rot)
{
    worldRotation += rot;
    localRotation += rot;
    hasChanged = true;
}

void Transform::AddScale(const lm::FVec3& scl)
{
    worldScale += scl;
    localScale += scl;
    hasChanged = true;
}

lm::FVec3 Transform::GetFront()
{
    const lm::FVec4 temp = GetMatrix()[2];
    const lm::FVec3 forward(temp[0], temp[1], temp[2]);

    return lm::FVec3::Normalize(forward);
}

lm::FVec3 Transform::GetRight()
{
    const lm::FVec4 temp = GetMatrix()[0];
    const lm::FVec3 right(temp[0], temp[1], temp[2]);

    return lm::FVec3::Normalize(right);
}

lm::FVec3 Transform::GetUp()
{
    const lm::FVec4 temp = GetMatrix()[1];
    const lm::FVec3 up(temp[0], temp[1], temp[2]);

    return lm::FVec3::Normalize(up);
}

lm::FMat4 Transform::GetMatrix()
{
    if (hasChanged)
        UpdateMatrix();

    return worldMatrix;
}

void Transform::UpdateMatrix()
{
    worldMatrix = lm::FMat4::Transform(worldPosition, worldRotation, worldScale);
    hasChanged = false;
}

void Transform::LimitRotation()
{
    for (int i = 0; i < 3; i++)
    {
        if (worldRotation[i] > 360 || worldRotation[i] < -360)
            worldRotation[i] = std::fmod(worldRotation[i], 360);
    }
}