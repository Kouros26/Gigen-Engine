#pragma once
#include "Vec3/FVec3.hpp"
#include "Mat4/FMat4.hpp"
#include <Quaternion/FQuat.hpp>

class Transform
{
public:
    Transform();
    ~Transform();

    void SetWorldPosition(const lm::FVec3& pos);
    void SetWorldRotation(const lm::FVec3& rot);
    void SetWorldScale(const lm::FVec3& scl);

    void SetLocalPosition(const lm::FVec3& pos);
    void SetLocalRotation(const lm::FVec3& rot);
    void SetLocalScale(const lm::FVec3& scl);

    [[nodiscard]] lm::FVec3 GetWorldPosition() const;
    [[nodiscard]] lm::FVec3 GetWorldScale() const;
    [[nodiscard]] lm::FVec3 GetWorldRotation() const;
    [[nodiscard]] lm::FQuat GetOrientation();

    [[nodiscard]] lm::FVec3 GetLocalPosition() const;
    [[nodiscard]] lm::FVec3 GetLocalScale() const;
    [[nodiscard]] lm::FVec3 GetLocalRotation() const;

    void AddScale(const lm::FVec3& scl);
    void AddRotation(const lm::FVec3& rot);
    void AddPosition(const lm::FVec3& pos);

    lm::FVec3 GetFront();
    lm::FVec3 GetUp();
    lm::FVec3 GetRight();

    lm::FMat4 GetMatrix();

private:
    void UpdateMatrix();
    void LimitRotation();
    //

    lm::FVec3 localScale = lm::FVec3(1);
    lm::FVec3 localPosition;
    lm::FVec3 localRotation = lm::FVec3(1.f);

    lm::FVec3 worldScale = lm::FVec3(1);
    lm::FVec3 worldPosition;
    lm::FVec3 worldRotation = lm::FVec3(1.f);

    lm::FMat4 worldMatrix;

    bool hasChanged = true;
};