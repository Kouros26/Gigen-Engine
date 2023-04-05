#include "Shader.h"
#include <fstream>
#include "Renderer.h"
#include "Mat4/FMat4.hpp"
#include <iostream >

Shader::Shader(std::string const& filePath, int shaderType)
    :IResource(filePath)
{
    this->shaderType = shaderType;
    std::string str = readFile(filePath);
    if (str == "") {
        std::cout << "no fragment file at path " << filePath << std::endl;
        return;
    }
    this->shader = str;
}

void Shader::Init()
{
    shaderId = RENDERER.CreateShader(shaderType);
    const char* content = shader.c_str();

    RENDERER.ShaderSource(shaderId, 1, &content, NULL);
    RENDERER.CompileShader(shaderId);

    int success = RD_FALSE;
    char infoLog[512];
    RENDERER.GetShaderiv(shaderId, RD_COMPILE_STATUS, &success);

    if (success == RD_FALSE)
    {
        RENDERER.GetShaderInfoLog(shaderId, 512, NULL, infoLog);

        std::cout << "error compiling fragment : " << filePath << std::endl;
        std::cout << infoLog << std::endl;
        return;
    }
}

VertexShader::VertexShader(std::string const& filePath)
    :Shader(filePath, RD_VERTEX_SHADER)
{
}

FragmentShader::FragmentShader(std::string const& filePath)
    : Shader(filePath, RD_FRAGMENT_SHADER)
{
}

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
    if (shaderProgram != RD_FALSE)
        RENDERER.DeleteProgram(shaderProgram);
}

bool ShaderProgram::Link(VertexShader* vertex, FragmentShader* fragment)
{
    if (shaderProgram == RD_FALSE)
        shaderProgram = RENDERER.CreateProgram();

    if (vertex->shaderId == RD_FALSE || fragment->shaderId == RD_FALSE) return false;

    RENDERER.AttachShader(shaderProgram, vertex->shaderId);
    RENDERER.AttachShader(shaderProgram, fragment->shaderId);
    RENDERER.LinkProgram(shaderProgram);

    int success = RD_TRUE;
    char infoLog[512]{ 0 };
    RENDERER.GetShaderiv(shaderProgram, RD_LINK_STATUS, &success);
    if (success == RD_FALSE) {
        int logLen;
        RENDERER.GetProgramiv(shaderProgram, RD_INFO_LOG_LENGTH, &logLen);
        int written;
        RENDERER.GetProgramInfoLog(shaderProgram, logLen, &written, infoLog);
        std::cout << "error linking program : " << std::endl;
        std::cout << infoLog << std::endl;
        return false;
    }

    return true;
}

void ShaderProgram::Use()
{
    if (shaderProgram != RD_FALSE)
        RENDERER.UseProgram(shaderProgram);
}

void ShaderProgram::UnUse()
{
    RENDERER.UseProgram(0);
}

unsigned int ShaderProgram::GetId()
{
    return shaderProgram;
}

unsigned int ShaderProgram::GetUniform(const char* name)
{
    unsigned int result = RENDERER.GetUniformLocation(shaderProgram, name);

    if (result == -1)
        std::cout << name << " not found in uniform" << std::endl;

    return result;
}

void ShaderProgram::SetVec3(float vec[3], const char* name)
{
    RENDERER.SetUniformValue(shaderProgram, name, UniformType::VEC3, vec);
}

void ShaderProgram::SetMat4(lm::FMat4& value, const char* name)
{
    RENDERER.SetUniformValue(shaderProgram, name, UniformType::MAT4, lm::FMat4::ToArray(value));
}

void ShaderProgram::SetBool(bool& value, const char* name)
{
    RENDERER.SetUniformValue(shaderProgram, name, UniformType::BOOL, &value);
}

void ShaderProgram::SetInt(int& value, const char* name)
{
    RENDERER.SetUniformValue(shaderProgram, name, UniformType::INT, &value);
}

void ShaderProgram::SetFloat(float& value, const char* name)
{
    RENDERER.SetUniformValue(shaderProgram, name, UniformType::FLOAT, &value);
}