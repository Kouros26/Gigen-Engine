#include "Camera.h"
#include "Application.h"

Camera::Camera()
	:GameObject("Camera")
{
	CreateViewMatrix();

	currentRatio = Application::GetWindow().GetRatio();
	UpdateProjectionMatrix();
}

Camera::~Camera()
{
}

lm::FMat4 Camera::CreateViewMatrix()
{
	view = view.LookAt(transform.GetPosition(), transform.GetPosition() + transform.GetFront(), -transform.GetUp());
	return view;
}

lm::FMat4 Camera::GetViewMatrix() const
{
	return view;
}

void Camera::SetFov(float fov)
{
	currentFov = fov;
	UpdateProjectionMatrix();
}

void Camera::SetNear(float near)
{
	currentNear = near;
	UpdateProjectionMatrix();
}

void Camera::SetFar(float far)
{
	currentFar = far;
	UpdateProjectionMatrix();
}

void Camera::SetRatio(float ratio)
{
	currentRatio = ratio;
	UpdateProjectionMatrix();
}

lm::FMat4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::UpdateProjectionMatrix()
{
	projectionMatrix = lm::FMat4::Perspective(currentFov, currentRatio, currentNear, currentFar);
	projectionMatrix[1][1] *= -1;
}