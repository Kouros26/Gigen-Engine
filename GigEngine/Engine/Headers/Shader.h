#pragma once
#include "IResource.h"
#include <glad/glad.h>
#include <string>

class Shader : public IResource
{
public:
	Shader(std::string const& filename, int shaderType);

	static bool Link(class VertexShader& vertex, class FragmentShader& fragment);
	static void Use();

private:
	inline static GLuint shaderProgram = GL_FALSE;

protected:
	GLuint shaderId = GL_FALSE;
};

class VertexShader : public Shader
{
public:
	VertexShader(std::string const& filename);
};

class FragmentShader : public Shader
{
public:
	FragmentShader(std::string const& filename);
};