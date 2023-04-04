#include <glad/glad.h>
#include "Mesh.h"

Mesh::Mesh(unsigned int verticesSize, unsigned int indicesSize)
    :verticesSize(verticesSize), indicesSize(indicesSize)
{
}

Mesh::Mesh(const Mesh& other)
{
    this->verticesSize = other.verticesSize;
    this->indicesSize = other.indicesSize;

    this->vertices = new float[verticesSize];
    this->indices = new unsigned int[indicesSize];

    for (int i = 0; i < verticesSize; i++)
        this->vertices[i] = other.vertices[i];

    for (int i = 0; i < indicesSize; i++)
        this->indices[i] = other.indices[i];

    setUpBuffers();
}

Mesh::Mesh(Mesh&& other) noexcept
{
    this->verticesSize = other.verticesSize;
    this->indicesSize = other.indicesSize;

    this->vertices = other.vertices;
    this->indices = other.indices;

    this->VAO = other.VAO;
    this->VBO = other.VBO;
    this->EBO = other.EBO;

    other.vertices = nullptr;
    other.indices = nullptr;
    other.VAO = GL_FALSE;
    other.VBO = GL_FALSE;
    other.EBO = GL_FALSE;

    other.verticesSize = 0;
    other.indicesSize = 0;
}

Mesh::~Mesh()
{
    delete[] vertices;
    delete[] indices;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

Mesh& Mesh::operator=(const Mesh& other)
{
    if (this != &other)
    {
        this->verticesSize = other.verticesSize;
        this->indicesSize = other.indicesSize;

        this->vertices = new float[verticesSize];
        this->indices = new unsigned int[indicesSize];

        for (int i = 0; i < verticesSize; i++)
            this->vertices[i] = other.vertices[i];

        for (int i = 0; i < indicesSize; i++)
            this->indices[i] = other.indices[i];

        setUpBuffers();
    }
    return *this;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        this->verticesSize = other.verticesSize;
        this->indicesSize = other.indicesSize;

        this->vertices = other.vertices;
        this->indices = other.indices;

        this->VAO = other.VAO;
        this->VBO = other.VBO;
        this->EBO = other.EBO;

        other.vertices = nullptr;
        other.indices = nullptr;
        other.VAO = GL_FALSE;
        other.VBO = GL_FALSE;
        other.EBO = GL_FALSE;

        other.verticesSize = 0;
        other.indicesSize = 0;
    }
    return *this;
}

void Mesh::Draw()
{
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setUpBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);       // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);       // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);       // textureCoordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}