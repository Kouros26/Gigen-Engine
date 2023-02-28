#include "EditorCamera.h"
#include "Application.h"
#include "Inputs.h"
#include "Watch.h"

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
		{
			speed = speedMax;
		}
		pressLeftShift = true;
	}
	if (!Inputs::GetKey(LEFT_SHIFT))
	{
		pressLeftShift = false;
	}

	if (Inputs::GetKey(RIGHT_SHIFT) && !pressRightShift)
	{
		speed -= speedStep;
		if (speed < 0)
		{
			speed = 0;
		}
		pressRightShift = true;
	}
	if (!Inputs::GetKey(RIGHT_SHIFT))
	{
		pressRightShift = false;
	}
}

void EditorCamera::Move()
{
	const float scaleSpeed = speed * static_cast<float>(Time::GetDeltaTime());

	if (Inputs::GetKey(UP) || Inputs::GetKey('W'))
	{
		transform.AddPosition(GetFront() * scaleSpeed);
	}
	if (Inputs::GetKey(DOWN) || Inputs::GetKey('S'))
	{
		transform.AddPosition(GetFront() * -scaleSpeed);
	}
	if (Inputs::GetKey(LEFT) || Inputs::GetKey('A'))
	{
		transform.AddPosition(-GetRight() * -scaleSpeed);
	}
	if (Inputs::GetKey(RIGHT) || Inputs::GetKey('D'))
	{
		transform.AddPosition(-GetRight() * scaleSpeed);
	}

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
		const float Ry = static_cast<float>(-Inputs::GetMouse().mouseOffsetX * static_cast<double>(sensitivity));
		const float Rx = static_cast<float>(-Inputs::GetMouse().mouseOffsetY * static_cast<double>(sensitivity) );

		transform.AddRotation(lm::FVec3(-Rx, Ry, 0));

		lm::FVec3 rot = transform.GetRotation();
		if (rot.x > maxLookAngle)
			transform.SetRotation(lm::FVec3(maxLookAngle, rot.y, rot.z));

		if (rot.x < -maxLookAngle)
			transform.SetRotation(lm::FVec3(-maxLookAngle, rot.y, rot.z));
	}

	else if (Inputs::GetMouse().rightClick == 0)
		glfwSetInputMode(Application::GetWindow().GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}