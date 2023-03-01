#pragma once
#include "Vec3/FVec3.hpp"
#include "Mat4/FMat4.hpp"
#include <Quaternion/FQuat.hpp>

class Transform
{
public:
    Transform();
    ~Transform();

    void SetPosition(const lm::FVec3& pos);
    void SetRotation(const lm::FQuat& rot);
    void SetScale(const lm::FVec3& scl);

    [[nodiscard]] lm::FVec3 GetPosition() const;
    [[nodiscard]] lm::FVec3 GetScale() const;
    [[nodiscard]] lm::FQuat GetRotation() const;

    void AddScale(const lm::FVec3& scl);
    void AddRotation(const lm::FQuat& rot);
    void AddPosition(const lm::FVec3& pos);

    lm::FVec3 GetFront();
    lm::FVec3 GetUp();
    lm::FVec3 GetRight();

    lm::FMat4 GetMatrix();

private:
    void UpdateMatrix();
    void LimitRotation();
    //

    lm::FVec3 scale = lm::FVec3(1);
    lm::FVec3 position;
    lm::FQuat rotation = lm::FQuat(1.f);
    lm::FMat4 matrix;

    bool hasChanged = true;
};