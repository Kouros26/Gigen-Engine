#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
    Camera();
    ~Camera();

    lm::FMat4 CreateViewMatrix();
    lm::FMat4 GetViewMatrix() const;

    void SetFov(float fov);
    void SetNear(float near);
    void SetFar(float far);
    void SetRatio(float ratio);
    lm::FMat4 GetProjectionMatrix();

private:
    void UpdateProjectionMatrix();

    lm::FMat4 view;

    lm::FMat4 projectionMatrix;
    float currentFov = 90;
    float currentNear = 0.1f;
    float currentFar = 1000;
    float currentRatio;
};
