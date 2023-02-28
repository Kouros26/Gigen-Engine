#pragma once
#include "IResource.h"
#include <glad/glad.h>
#include <string>
#include "Mat4/FMat4.hpp"
#include "Vec3/FVec3.hpp"

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
	bool Link(class VertexShader* vertex, class FragmentShader* fragment);
	void Use();

	GLuint GetId();
	GLuint GetUniform(const char* name);

	void SetVec3(float vec[3], const char* name);
	void SetMat4(lm::FMat4& value, const char* name);
	void SetBool(bool& value, const char* name);
	void SetInt(int& value, const char* name);
	void SetFloat(float& value, const char* name);

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