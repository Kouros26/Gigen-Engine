#pragma once
#include "IResource.h"
#include <glad/glad.h>
#include <string>

class ShaderProgram : public IResource
{
public:
	ShaderProgram();
	~ShaderProgram();
	bool Link(class VertexShader& vertex, class FragmentShader& fragment);
	void Use();

	GLuint GetId();

private:
	GLuint shaderProgram = GL_FALSE;
};

class Shader : public IResource
{
public:
	Shader(std::string const& filename, int shaderType);
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