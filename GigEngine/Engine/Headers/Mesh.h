#pragma once
#include "IResource.h"
#include "Vec3/FVec3.hpp"
#include "Vec2/FVec2.hpp"
#include <vector>
#include "GLFW/glfw3.h"

constexpr int VERTEX_SIZE = 8; //8 is 3 pos + 3 normal + 2 texture
constexpr int FACE_SIZE = 3; //triangle

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
