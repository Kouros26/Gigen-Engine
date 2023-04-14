#include "Renderer.h"
#include "DrawLine.h"
#include "Watch.h"
#include "Application.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"

using namespace GigRenderer;

Line::Line(const lm::FVec3 start, const lm::FVec3 end, const lm::FVec3 color, float timer)
    :timer(timer)
{
    vertices[0] = start.x;
    vertices[1] = start.y;
    vertices[2] = start.z;

    vertices[3] = end.x;
    vertices[4] = end.y;
    vertices[5] = end.z;

    this->color[0] = color.x;
    this->color[1] = color.y;
    this->color[2] = color.z;

    Buffer VBO{ this->VBO, vertices, sizeof(vertices) };
    BufferVAO VAO{ this->VAO };
    RENDERER.SetupBuffer(VBO, VAO);
}

Line::~Line()
{
    RENDERER.DeleteVertexArray(1, &VAO);
    RENDERER.DeleteBuffer(1, &VBO);
}

unsigned int Line::GetVAO()
{
    return VAO;
}

float* Line::GetColor()
{
    return color;
}

void Lines::Init()
{
    VertexShader* mainVertex = ResourceManager::Get<VertexShader>("Resources/Shaders/vertLine.vert");
    FragmentShader* mainFragment = ResourceManager::Get<FragmentShader>("Resources/Shaders/fragLine.frag");

    if (!shaderProgram.Link(mainVertex, mainFragment))
        std::cout << "Error linking drawLine shader" << std::endl;

    MVPLocation = RENDERER.GetUniformLocation(shaderProgram.GetId(), "MVP");
    colorLocation = RENDERER.GetUniformLocation(shaderProgram.GetId(), "color");
}

void Lines::DrawLine(const lm::FVec3& start, const lm::FVec3& end, const lm::FVec3& color, float timer)
{
    debugLines.push_back(new Line(start, end, color, timer));
}

void Lines::DrawLines()
{
    shaderProgram.Use();

    RENDERER.SetUniformValue(MVPLocation, UniformType::MAT4, lm::FMat4::ToArray(Application::GetViewProj()));

    DrawDebugLines();
    RENDERER.Disable(RD_DEPTH_TEST);
    DrawGuizmoLines();
    shaderProgram.UnUse();
}

void Lines::DrawDebugLines()
{
    for (int i = 0; i < debugLines.size(); i++)
    {
        if (debugLines[i])
        {
            debugLines[i]->timer -= Time::GetDeltaTime();
            if (debugLines[i]->timer < 0)
            {
                delete debugLines[i];
                debugLines.erase(debugLines.begin() + i);
                i--;
                continue;
            }

            RENDERER.SetUniformValue(colorLocation, UniformType::VEC3, debugLines[i]->GetColor());

            RENDERER.BindVertexArray(debugLines[i]->GetVAO());
            RENDERER.DrawArray(RD_LINES, 0, 2);
            RENDERER.BindVertexArray(0);
        }
    }
}

void Lines::Clear()
{
    for (int i = 0; i < debugLines.size(); i++)
    {
        delete debugLines[i];
    }
    debugLines.clear();
}

void Lines::DrawGuizmoLines()
{
	CreateTranslatedEditorTransform();
	CreateGuizmo(&worldTransform);

	GameObject* obj = GameObjectManager::GetFocusedGameObject();
	if (obj)
	{
		CreateGuizmo(&obj->GetTransform());
	}

	for (int i = 0; i < guizmoLines.size(); i++)
	{
		if (guizmoLines[i])
		{
			glUniform3fv(colorLocation, 1, guizmoLines[i]->GetColor());

            RENDERER.BindVertexArray(guizmoLines[i]->GetVAO());

            RENDERER.DrawArray(RD_LINES, 0, 2);

            RENDERER.BindVertexArray(0);
        }
    }

    for (int i = 0; i < guizmoLines.size(); i++)
    {
        delete guizmoLines[i];
    }
    guizmoLines.clear();
}

void Lines::CreateGuizmo(Transform* t)
{
    if (!t) return;

    const lm::FVec3 pos(t->GetWorldPosition());
    guizmoLines.push_back(new Line(pos, pos + t->GetRight(), lm::FVec3(1, 0, 0), 0));
    guizmoLines.push_back(new Line(pos, pos + t->GetUp(), lm::FVec3(0, 1, 0), 0));
    guizmoLines.push_back(new Line(pos, pos + t->GetFront(), lm::FVec3(0, 0, 1), 0));
}

void Lines::CreateTranslatedEditorTransform()
{
    //this must change, but ok for now
    lm::FVec3 pos = Application::GetEditorCamera().GetTransform().GetWorldPosition();
    pos += 10 * Application::GetEditorCamera().GetFront();
    pos -= 10 * Application::GetEditorCamera().GetRight();
    pos += 8 * Application::GetEditorCamera().GetUp();

    worldTransform.SetWorldPosition(pos);
}