#pragma once
#include "Camera.h"

class EditorCamera : public Camera
{
public:
    EditorCamera();
    ~EditorCamera();

    virtual void Update() override;

private:
    void ChangeSpeed();
    void Move();
    void Look();

    float speed = 2;
    float speedStep = 2;
    float speedMax = 50;
    float sensitivity = 2;

    float maxLookAngle = 80;

    bool pressLeftShift = false;
    bool pressRightShift = false;
};
