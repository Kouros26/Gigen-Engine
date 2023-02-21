#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
    Camera();
    ~Camera();

    lm::FMat4 CreateViewMatrix();
    lm::FMat4 GetViewMatrix() const;

    static void SetFov(float fov);
    static void SetNear(float near);
    static void SetFar(float far);
    static void SetRatio(float ratio);
    static lm::FMat4 GetProjectionMatrix();

private:
    static void UpdateProjectionMatrix();

    lm::FMat4 view;

    inline static lm::FMat4 projectionMatrix;
    inline static float currentFov;
    inline static float currentNear;
    inline static float currentFar;
    inline static float currentRatio;
};

inline void Camera::SetFov(float fov)
{
    currentFov = fov;
    UpdateProjectionMatrix();
}

inline void Camera::SetNear(float near)
{
    currentNear = near;
    UpdateProjectionMatrix();
}

inline void Camera::SetFar(float far)
{
    currentFar = far;
    UpdateProjectionMatrix();
}

inline void Camera::SetRatio(float ratio)
{
    currentRatio = ratio;
    UpdateProjectionMatrix();
}

inline lm::FMat4 Camera::GetProjectionMatrix()
{
    return projectionMatrix;
}

inline void Camera::UpdateProjectionMatrix()
{
    projectionMatrix = lm::FMat4::Perspective(currentFov, currentRatio, currentNear, currentFar);
}
