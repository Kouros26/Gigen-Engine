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
}

void Transform::SetRotation(lm::FVec3 rot)
{
    rotation = rot;
    ClampRotation();
}

void Transform::SetScale(lm::FVec3 scl)
{
    scale = scl;
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
}

lm::FVec3 Transform::GetFront()
{
    lm::FMat4 inverse = lm::FMat4::Inverse(GetMatrix());
    lm::FVec4 temp = inverse[2];
    lm::FVec3 forward(temp[0], temp[1], temp[2]);

    return lm::FVec4::Normalize(-forward);
}

lm::FVec3 Transform::GetRight()
{
    lm::FMat4 inverse = lm::FMat4::Inverse(GetMatrix());
    lm::FVec4 temp = inverse[0];
    lm::FVec3 forward(temp[0], temp[1], temp[2]);

    return lm::FVec4::Normalize(forward);
}

lm::FVec3 Transform::GetUp()
{
    lm::FMat4 inverse = lm::FMat4::Inverse(GetMatrix());
    lm::FVec4 temp = inverse[1];
    lm::FVec3 forward(temp[0], temp[1], temp[2]);

    return lm::FVec4::Normalize(forward);
}

lm::FMat4 Transform::GetMatrix()
{
    UpdateMatrix();
    return matrix;
}

void Transform::AddRotation(lm::FVec3 rot)
{
    rotation += rot;
    ClampRotation();
}

void Transform::AddScale(lm::FVec3 scl)
{
    scale += scl;
}

void Transform::UpdateMatrix()
{
    matrix = lm::FMat4::Transform(position, rotation, scale);
}

void Transform::ClampRotation()
{
    for (int i = 0; i < 3; i++)
    {
        while (rotation[i] < 0)
        {
            rotation[i] += 360;
        }
        while (rotation[i] > 360)
        {
            rotation[i] -= 360;
        }
    }
}