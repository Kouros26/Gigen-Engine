#include "Transform.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::SetPosition(lm::FVec3 pos)
{
	position = pos;
	hasChanged = true;
}

void Transform::SetRotation(lm::FVec3 rot)
{
	rotation = rot;
	LimitRotation();
	hasChanged = true;
}

void Transform::SetScale(lm::FVec3 scl)
{
	scale = scl;
	hasChanged = true;
}

lm::FVec3 Transform::GetPosition() const
{
	return position;
}

lm::FVec3 Transform::GetScale() const
{
	return scale;
}

lm::FVec3 Transform::GetRotation() const
{
	return rotation;
}

void Transform::AddPosition(lm::FVec3 pos)
{
	position += pos;
	hasChanged = true;
}

lm::FVec3 Transform::GetFront()
{
	const lm::FVec4 temp = GetMatrix()[2];
	lm::FVec3 forward(temp[0], temp[1], temp[2]);

	return lm::FVec3::Normalize(forward);
}

lm::FVec3 Transform::GetRight()
{
	const lm::FVec4 temp = GetMatrix()[0];
	lm::FVec3 right(temp[0], temp[1], temp[2]);

	return lm::FVec3::Normalize(right);
}

lm::FVec3 Transform::GetUp()
{
	const lm::FVec4 temp = GetMatrix()[1];
	lm::FVec3 up(temp[0], temp[1], temp[2]);

	return lm::FVec3::Normalize(up);
}

lm::FMat4 Transform::GetMatrix()
{
	if (hasChanged)
		UpdateMatrix();

	return matrix;
}

void Transform::AddRotation(lm::FVec3 rot)
{
	rotation += rot;
	LimitRotation();
	hasChanged = true;
}

void Transform::AddScale(lm::FVec3 scl)
{
	scale += scl;
	hasChanged = true;
}

void Transform::UpdateMatrix()
{
	matrix = lm::FMat4::Transform(position, rotation, scale);
	hasChanged = false;
}

void Transform::LimitRotation()
{
	for (int i = 0; i < 3; i++)
	{
		while (rotation[i] < -360)
		{
			rotation[i] += 360;
		}
		while (rotation[i] > 360)
		{
			rotation[i] -= 360;
		}
	}
}