#include "Shader.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

Shader::Shader(std::string const& filePath, int shaderType)
	:IResource(filePath)
{
	std::string str = readFile(filePath);
	if (str == "") {
		std::cout << "no fragment file at path " << filePath << std::endl;
		return;
	}
	shaderId = glCreateShader(shaderType);
	const char* content = str.c_str();

	glShaderSource(shaderId, 1, &content, NULL);
	glCompileShader(shaderId);

	GLint success = GL_FALSE;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "error compiling fragment : " << filePath << std::endl;
		std::cout << infoLog << std::endl;
		return;
	}
}

VertexShader::VertexShader(std::string const& filePath)
	:Shader(filePath, GL_VERTEX_SHADER)
{
}

FragmentShader::FragmentShader(std::string const& filePath)
	: Shader(filePath, GL_FRAGMENT_SHADER)
{
}

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(shaderProgram);
}

bool ShaderProgram::Link(VertexShader* vertex, FragmentShader* fragment)
{
	if (shaderProgram == GL_FALSE)
		shaderProgram = glCreateProgram();

	if (vertex->shaderId == GL_FALSE || fragment->shaderId == GL_FALSE) return false;

	glAttachShader(shaderProgram, vertex->shaderId);
	glAttachShader(shaderProgram, fragment->shaderId);
	glLinkProgram(shaderProgram);

	GLint success = GL_TRUE;
	char infoLog[512]{ 0 };
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logLen;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
		GLsizei written;
		glGetProgramInfoLog(shaderProgram, 512, &written, infoLog);
		std::cout << "error linking program : " << std::endl;
		std::cout << infoLog << std::endl;
		return false;
	}

	return true;
}

void ShaderProgram::Use()
{
	if (shaderProgram != GL_FALSE)
		glUseProgram(shaderProgram);
}

GLuint ShaderProgram::GetId()
{
	return shaderProgram;
}

void ShaderProgram::SetVec3(lm::FVec3& value, const char* name)
{
	float f[3]{ value.x, value.y, value.z };
	glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, f);
}

void ShaderProgram::SetMat4(lm::FMat4& value, const char* name)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, lm::FMat4::ToArray(value));
}

void ShaderProgram::SetBool(bool& value, const char* name)
{
	glUniform1i(glGetUniformLocation(shaderProgram, name), (int)value);
}

void ShaderProgram::SetInt(int& value, const char* name)
{
	glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

void ShaderProgram::SetFloat(float& value, const char* name)
{
	glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}