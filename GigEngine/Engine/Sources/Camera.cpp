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

lm::mat4 Camera::CreateViewMatrix()
{
	view = view.lookAt(transform.GetPosition(), transform.GetPosition() + transform.GetFront(), transform.GetUp());
	return view;
}

lm::mat4 Camera::GetViewMatrix() const
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

lm::mat4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::UpdateProjectionMatrix()
{
	projectionMatrix = lm::mat4::perspectiveProjection(currentFov, currentRatio, currentNear, currentFar);
	projectionMatrix[1][1] *= -1;
}