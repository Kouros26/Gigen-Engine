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
        GetTransform().AddPosition(GetFront() * scaleSpeed);

    if (Inputs::GetKey(DOWN) || Inputs::GetKey('S'))
        GetTransform().AddPosition(GetFront() * -scaleSpeed);

    if (Inputs::GetKey(LEFT) || Inputs::GetKey('A'))
        GetTransform().AddPosition(-GetRight() * -scaleSpeed);

    if (Inputs::GetKey(RIGHT) || Inputs::GetKey('D'))
        GetTransform().AddPosition(-GetRight() * scaleSpeed);

    if (Inputs::GetMouse().wheelClick)
    {
        GetTransform().AddPosition(GetRight() * Inputs::GetMouse().mouseOffsetX);
        GetTransform().AddPosition(GetUp() * -Inputs::GetMouse().mouseOffsetY);
    }

    if (Inputs::GetMouse().wheelOffsetY != 0)
    {
        GetTransform().AddPosition(GetFront() * Inputs::GetMouse().wheelOffsetY);
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

        const lm::FVec3 rot = GetTransform().GetWorldRotation();

        lm::FVec3 vecRot = { -Rx, Ry, 0 };
        GetTransform().AddRotation(vecRot);

        if (rot.x > maxLookAngle)
        {
            lm::FVec3 vecRot = { maxLookAngle, rot.y, rot.z };
            GetTransform().SetWorldRotation(vecRot);
        }

        if (rot.x < -maxLookAngle)
        {
            lm::FVec3 vecRot = { -maxLookAngle, rot.y, rot.z };
            GetTransform().SetWorldRotation(vecRot);
        }
    }

    else if (Inputs::GetMouse().rightClick == 0)
        glfwSetInputMode(Application::GetWindow().GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}