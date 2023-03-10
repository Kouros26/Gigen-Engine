#include "DrawLine.h"
#include "Watch.h"
#include "Application.h"
#include "ResourceManager.h"

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

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Line::~Line()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

GLuint Line::GetVAO()
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

	MVPLocation = glGetUniformLocation(shaderProgram.GetId(), "MVP");
	colorLocation = glGetUniformLocation(shaderProgram.GetId(), "color");
}

void Lines::DrawLine(const lm::FVec3& start, const lm::FVec3& end, const lm::FVec3& color, float timer)
{
	debugLines.push_back(new Line(start, end, color, timer));
}

void Lines::SetFocusedObjectTransform(Transform* transform)
{
	focusedObjectTransform = transform;
}

void Lines::DrawLines()
{
	shaderProgram.Use();
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, lm::FMat4::ToArray(Application::GetViewProj()));

	DrawDebugLines();
	glDisable(GL_DEPTH_TEST);
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

			glUniform3fv(colorLocation, 1, debugLines[i]->GetColor());

			glBindVertexArray(debugLines[i]->GetVAO());
			glDrawArrays(GL_LINES, 0, 2);
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
	CreateGuizmo(focusedObjectTransform);

	for (int i = 0; i < guizmoLines.size(); i++)
	{
		if (guizmoLines[i])
		{
			glUniform3fv(colorLocation, 1, guizmoLines[i]->GetColor());

			glBindVertexArray(guizmoLines[i]->GetVAO());
			glDrawArrays(GL_LINES, 0, 2);
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