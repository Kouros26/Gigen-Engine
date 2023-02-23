#pragma once
#include "IResource.h"
#include <glad/glad.h>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
	bool Link(class VertexShader* vertex, class FragmentShader* fragment);
	void Use();

	GLuint GetId();

private:
	GLuint shaderProgram = GL_FALSE;
};

class Shader : public IResource
{
public:
	Shader(std::string const& filePath, int shaderType);
	GLuint shaderId = GL_FALSE;
};

class VertexShader : public Shader
{
public:
	VertexShader(std::string const& filePath);
};

class FragmentShader : public Shader
{
public:
	FragmentShader(std::string const& filePath);
};