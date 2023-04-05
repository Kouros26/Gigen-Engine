#include "Renderer.h"
#include "GLFW/glfw3.h"
#include <iostream>

Renderer& Renderer::GetInstance()
{
    static Renderer instance;
    return instance;
}

void Renderer::Init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }
}

void Renderer::DeleteVertexArray(int n, const unsigned int* array)
{
    glDeleteVertexArrays(n, array);
}

void Renderer::DeleteBuffer(int n, const unsigned int* pBuffer)
{
    glDeleteBuffers(n, pBuffer);
}

void Renderer::BindVertexArray(const unsigned int pArray)
{
    glBindVertexArray(pArray);
}

void Renderer::BindBuffer(unsigned int pTarget, unsigned int pBuffer)
{
    glBindBuffer(pTarget, pBuffer);
}

void Renderer::DrawElements(unsigned int pMode, int pCount, unsigned int pType, const void* pIndices)
{
    glDrawElements(pMode, pCount, pType, pIndices);
}

void Renderer::GenVertexArrays(int n, unsigned int* array)
{
    glGenVertexArrays(n, array);
}

void Renderer::GenBuffers(int n, unsigned int* buffer)
{
    glGenBuffers(n, buffer);
}

void Renderer::BufferData(unsigned int pTarget, int pSize, const void* pData, unsigned int pUsage)
{
    glBufferData(pTarget, pSize, pData, pUsage);
}

void Renderer::EnableVertexAttribArray(unsigned int pIndex)
{
    glEnableVertexAttribArray(pIndex);
}

void Renderer::VertexAttribPointer(unsigned int pIndex, int pSize, unsigned int pType, bool pNormalized, int pStride, const void* pPointer)
{
    glVertexAttribPointer(pIndex, pSize, pType, pNormalized, pStride, pPointer);
}

void Renderer::DisableVertexAttribArray(unsigned int pIndex)
{
    glDisableVertexAttribArray(pIndex);
}

unsigned int Renderer::CreateShader(unsigned int shaderType)
{
    return glCreateShader(shaderType);
}

void Renderer::ShaderSource(unsigned int shader, int count, const char** string, const int* length)
{
    glShaderSource(shader, count, string, length);
}

void Renderer::CompileShader(unsigned int shader)
{
    glCompileShader(shader);
}

void Renderer::GetShaderiv(unsigned int pShader, unsigned int pName, int* pParams)
{
    glGetShaderiv(pShader, pName, pParams);
}

void Renderer::GetShaderInfoLog(unsigned int pShader, int pBufSize, int* pLength, char* pInfoLog)
{
    glGetShaderInfoLog(pShader, pBufSize, pLength, pInfoLog);
}

void Renderer::ViewPort(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void Renderer::DeleteShader(unsigned int pShader)
{
    glDeleteShader(pShader);
}

void Renderer::DeleteProgram(unsigned int pProgram)
{
    glDeleteProgram(pProgram);
}

void Renderer::UseProgram(unsigned int pProgram)
{
    glUseProgram(pProgram);
}

unsigned int Renderer::CreateProgram()
{
    return glCreateProgram();
}

void Renderer::AttachShader(unsigned int pProgram, unsigned int pShader)
{
    glAttachShader(pProgram, pShader);
}

void Renderer::LinkProgram(unsigned int pProgram)
{
    glLinkProgram(pProgram);
}

void Renderer::GetProgramiv(unsigned int pProgram, unsigned int pName, int* pParams)
{
    glGetProgramiv(pProgram, pName, pParams);
}

void Renderer::GetProgramInfoLog(unsigned int pProgram, int pBufSize, int* pLength, char* pInfoLog)
{
    glGetProgramInfoLog(pProgram, pBufSize, pLength, pInfoLog);
}

int Renderer::GetUniformLocation(unsigned int pProgram, const char* pName)
{
    return glGetUniformLocation(pProgram, pName);
}

void Renderer::SetUniformValue(unsigned int pProgram, const char* pName, UniformType pType, void* pValue)
{
    int location = GetUniformLocation(pProgram, pName);
    switch (pType)
    {
    case UniformType::FLOAT:
        glUniform1f(location, *(float*)pValue);
        break;
    case UniformType::INT:
        glUniform1i(location, *(int*)pValue);
        break;
    case UniformType::BOOL:
        glUniform1i(location, *(bool*)pValue);
        break;
    case UniformType::VEC3:
        glUniform3fv(location, 1, (float*)pValue);
        break;
    case UniformType::MAT4:
        glUniformMatrix4fv(location, 1, GL_FALSE, (float*)pValue);
        break;
    default:
        break;
    }
}

void Renderer::SetUniformValue(int pLocation, UniformType pType, void* pValue)
{
    switch (pType)
    {
    case UniformType::FLOAT:
        glUniform1f(pLocation, *(float*)pValue);
        break;
    case UniformType::INT:
        glUniform1i(pLocation, *(int*)pValue);
        break;
    case UniformType::BOOL:
        glUniform1i(pLocation, *(bool*)pValue);
        break;
    case UniformType::VEC3:
        glUniform3fv(pLocation, 1, (float*)pValue);
        break;
    case UniformType::MAT4:
        glUniformMatrix4fv(pLocation, 1, GL_FALSE, (float*)pValue);
        break;
    default:
        break;
    }
}

void Renderer::Disable(unsigned int pCap)
{
    glDisable(pCap);
}

void Renderer::Enable(unsigned int pCap)
{
    glEnable(pCap);
}

void Renderer::DrawArray(unsigned int pMode, int pFirst, int pCount)
{
    glDrawArrays(pMode, pFirst, pCount);
}

void Renderer::ClearColor(float pRed, float pGreen, float pBlue, float pAlpha)
{
    glClearColor(pRed, pGreen, pBlue, pAlpha);
}

void Renderer::Clear(unsigned int pMask)
{
    glClear(pMask);
}