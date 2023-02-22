#pragma once
#include "IResource.h"
#include "Vec3/Vec3.h"
#include "Vec2/Vec2.h"
#include <vector>
#include "GLFW/glfw3.h"

class Mesh : public IResource
{
public:
	Mesh(unsigned int verticesSize, unsigned int indicesSize);
	~Mesh();

	void Draw();
	void setUpBuffers();

	float* vertices = nullptr;
	unsigned int* indices = nullptr;

private:
	unsigned int verticesSize = 0;
	unsigned int indicesSize = 0;

	GLuint VBO = GL_FALSE;
	GLuint VAO = GL_FALSE;
	GLuint EBO = GL_FALSE;
};
