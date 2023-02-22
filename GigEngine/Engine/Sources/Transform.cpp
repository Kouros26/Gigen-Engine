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
    ClampRotation();
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
    lm::FMat4 inverse = lm::FMat4::Inverse(GetMatrix());
    lm::FVec4 temp = inverse[2];
    lm::FVec3 forward(temp[0], temp[1], temp[2]);

    return lm::FVec3::Normalize(forward);
}

lm::FVec3 Transform::GetRight()
{
    lm::FMat4 inverse = lm::FMat4::Inverse(GetMatrix());
    lm::FVec4 temp = inverse[0];
    lm::FVec3 forward(temp[0], temp[1], temp[2]);

    return lm::FVec3::Normalize(forward);
}

lm::FVec3 Transform::GetUp()
{
    lm::FMat4 inverse = lm::FMat4::Inverse(GetMatrix());
    lm::FVec4 temp = inverse[1];
    lm::FVec3 forward(temp[0], temp[1], temp[2]);

    return lm::FVec3::Normalize(-forward);
}

lm::FMat4 Transform::GetMatrix()
{
    if(hasChanged)
        UpdateMatrix();

    return matrix;
}

void Transform::AddRotation(lm::FVec3 rot)
{
    rotation += rot;
    ClampRotation();
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

//rename this function, clamp not good
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