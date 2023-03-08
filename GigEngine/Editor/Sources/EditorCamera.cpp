#include "EditorCamera.h"
#include "Application.h"
#include "Inputs.h"
#include "Watch.h"
#include <Quaternion/FQuat.hpp>
#include <Mat3/FMat3.hpp>
#include <Mat4/FMat4.hpp>

EditorCamera::EditorCamera()
{
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::Update()
{
	ChangeSpeed();
	Move();
	Look();
}

void EditorCamera::ChangeSpeed()
{
	if (Inputs::GetKey(LEFT_SHIFT) && !pressLeftShift)
	{
		speed *= speedStep;

		if (speed > speedMax)
			speed = speedMax;

		pressLeftShift = true;
	}

	if (!Inputs::GetKey(LEFT_SHIFT))
		pressLeftShift = false;

	if (Inputs::GetKey(RIGHT_SHIFT) && !pressRightShift)
	{
		speed -= speedStep;

		if (speed < 0)
			speed = 0;

		pressRightShift = true;
	}

	if (!Inputs::GetKey(RIGHT_SHIFT))
		pressRightShift = false;
}

void EditorCamera::Move()
{
	const float scaleSpeed = speed * static_cast<float>(Time::GetDeltaTime());

	if (Inputs::GetKey(UP) || Inputs::GetKey('W'))
		transform.AddPosition(GetFront() * scaleSpeed);

	if (Inputs::GetKey(DOWN) || Inputs::GetKey('S'))
		transform.AddPosition(GetFront() * -scaleSpeed);

	if (Inputs::GetKey(LEFT) || Inputs::GetKey('A'))
		transform.AddPosition(-GetRight() * -scaleSpeed);

	if (Inputs::GetKey(RIGHT) || Inputs::GetKey('D'))
		transform.AddPosition(-GetRight() * scaleSpeed);

	if (Inputs::GetMouse().wheelClick)
	{
		transform.AddPosition(GetRight() * Inputs::GetMouse().mouseOffsetX);
		transform.AddPosition(GetUp() * -Inputs::GetMouse().mouseOffsetY);
	}

	if (Inputs::GetMouse().wheelOffsetY != 0)
	{
		transform.AddPosition(GetFront() * Inputs::GetMouse().wheelOffsetY);
		Inputs::UpdateMouseWheelOffset(0);
	}
}

void EditorCamera::Look()
{
	if (Inputs::GetMouse().rightClick == 1)
	{
		glfwSetInputMode(Application::GetWindow().GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		const float Ry = static_cast<float>(Inputs::GetMouse().mouseOffsetX * static_cast<double>(sensitivity));
		const float Rx = static_cast<float>(Inputs::GetMouse().mouseOffsetY * static_cast<double>(sensitivity));

		const lm::FVec3 rot = transform.GetRotation();

		lm::FQuat Rot = lm::FQuat(-Rx, Ry, 0);

		lm::FQuat RotX(GetRight(), -Rx);
		lm::FQuat RotY(GetUp(), Ry);
		lm::FQuat newRot = RotX * RotY;
		newRot = newRot * Rot * lm::FQuat::Conjugate(newRot);
		lm::FVec3 vecRot(newRot.x, newRot.y, newRot.z);

		transform.AddRotation(vecRot);

		if (rot.x > maxLookAngle)
		{
			RotX = { GetRight(), maxLookAngle };
			RotY = { GetUp(), rot.y };
			lm::FQuat RotZ(GetFront(), rot.z);
			newRot = RotX * RotY * RotZ;
			newRot = newRot * Rot * lm::FQuat::Conjugate(newRot);
			vecRot = { newRot.x, newRot.y, newRot.z };
			transform.SetRotation(vecRot);
		}

		if (rot.x < -maxLookAngle)
		{
			RotX = { GetRight(), -maxLookAngle };
			RotY = { GetUp(), rot.y };
			lm::FQuat RotZ(GetFront(), rot.z);
			newRot = RotX * RotY * RotZ;
			newRot = newRot * Rot * lm::FQuat::Conjugate(newRot);
			vecRot = { newRot.x, newRot.y, newRot.z };
			transform.SetRotation(vecRot);
		}
	}

	else if (Inputs::GetMouse().rightClick == 0)
		glfwSetInputMode(Application::GetWindow().GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}