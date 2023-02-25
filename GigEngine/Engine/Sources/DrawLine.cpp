#include "DrawLine.h"
#include "Application.h"
#include "ResourceManager.h"

Line::Line(lm::FVec3 start, lm::FVec3 end, lm::FVec3 color)
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

Lines::Lines()
{
}

Lines::~Lines()
{
}

void Lines::Init()
{
	VertexShader* mainVertex = ResourceManager::Get<VertexShader>("Resources/Shaders/vertLine.vert");
	FragmentShader* mainFragment = ResourceManager::Get<FragmentShader>("Resources/Shaders/fragLine.frag");

	shaderProgram.Link(mainVertex, mainFragment);

	MVPLocation = glGetUniformLocation(shaderProgram.GetId(), "MVP");
	colorLocation = glGetUniformLocation(shaderProgram.GetId(), "color");
}

void Lines::AddLine(lm::FVec3 start, lm::FVec3 end, lm::FVec3 color)
{
	lines.push_back(new Line(start, end, color));
}

void Lines::DrawLines()
{
	shaderProgram.Use();
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, lm::FMat4::ToArray(Application::GetViewProj()));

	for (int i = 0; i < lines.size(); i++)
	{
		if (lines[i])
		{
			glUniform3fv(colorLocation, 1, lines[i]->GetColor());

			glBindVertexArray(lines[i]->GetVAO());
			glDrawArrays(GL_LINES, 0, 2);
		}
	}
}

void Lines::Clear()
{
	for (int i = 0; i < lines.size(); i++)
	{
		if (lines[i])
			delete lines[i];
	}
	lines.clear();
}