#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	~Camera();

	lm::mat4 CreateViewMatrix();
	lm::mat4 GetViewMatrix() const;

	void SetFov(float fov);
	void SetNear(float near);
	void SetFar(float far);
	void SetRatio(float ratio);
	lm::mat4 GetProjectionMatrix();

private:
	void UpdateProjectionMatrix();

	lm::mat4 view;

	lm::mat4 projectionMatrix;
	float currentFov = 90;
	float currentNear = 0.1f;
	float currentFar = 1000;
	float currentRatio;
};
