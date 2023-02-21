#include "Camera.h"

Camera::Camera()
    :GameObject("Camera")
{
    CreateViewMatrix();
}

Camera::~Camera()
{
}

lm::FMat4 Camera::CreateViewMatrix()
{
    view = lm::FMat4::LookAt(transform.GetPosition(), transform.GetPosition() + transform.GetFront(), transform.GetUp());
    return view;
}

lm::FMat4 Camera::GetViewMatrix() const
{
    return view;
}