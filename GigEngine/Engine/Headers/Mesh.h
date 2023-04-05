#pragma once
#include "IResource.h"
#include "Vec3/FVec3.hpp"
#include "Vec2/FVec2.hpp"
#include <vector>
#include "GLFW/glfw3.h"

constexpr int VERTEX_SIZE = 8; //8 is 3 pos + 3 normal + 2 texture
constexpr int FACE_SIZE = 3; //triangle

class Mesh
{
public:
    Mesh(unsigned int verticesSize, unsigned int indicesSize);
    ~Mesh();

    Mesh(const Mesh& other);
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(const Mesh& other);
    Mesh& operator=(Mesh&& other) noexcept;

    void Draw();
    void setUpBuffers();

    float* vertices = nullptr;
    unsigned int* indices = nullptr;

private:
    unsigned int verticesSize = 0;
    unsigned int indicesSize = 0;

    unsigned int VBO = 0;
    unsigned int VAO = 0;
    unsigned int EBO = 0;
};
