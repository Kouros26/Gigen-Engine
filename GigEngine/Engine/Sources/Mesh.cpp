#include "Renderer.h"
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
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;

    other.verticesSize = 0;
    other.indicesSize = 0;
}

Mesh::~Mesh()
{
    delete[] vertices;
    delete[] indices;
    RENDERER.DeleteVertexArray(1, &VAO);
    RENDERER.DeleteBuffer(1, &VBO);
    RENDERER.DeleteBuffer(1, &EBO);
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
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;

        other.verticesSize = 0;
        other.indicesSize = 0;
    }
    return *this;
}

void Mesh::Draw()
{
    RENDERER.BindVertexArray(VAO);
    RENDERER.DrawElements(RD_TRIANGLE, indicesSize, RD_UNSIGNED_INT, 0);
    RENDERER.BindVertexArray(0);
}

void Mesh::setUpBuffers()
{
    RENDERER.GenVertexArrays(1, &VAO);
    RENDERER.GenBuffers(1, &VBO);
    RENDERER.GenBuffers(1, &EBO);

    RENDERER.BindVertexArray(VAO);

    RENDERER.BindBuffer(RD_ARRAY_BUFFER, VBO);
    RENDERER.BufferData(RD_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, RD_STATIC_DRAW);

    RENDERER.BindBuffer(RD_ELEMENT_ARRAY_BUFFER, EBO);
    RENDERER.BufferData(RD_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), indices, RD_STATIC_DRAW);

    RENDERER.EnableVertexAttribArray(0);       // position
    RENDERER.VertexAttribPointer(0, 3, RD_FLOAT, RD_FALSE, VERTEX_SIZE * sizeof(float), (void*)0);

    RENDERER.EnableVertexAttribArray(1);       // normal
    RENDERER.VertexAttribPointer(1, 3, RD_FLOAT, RD_FALSE, VERTEX_SIZE * sizeof(float), (void*)(3 * sizeof(float)));

    RENDERER.EnableVertexAttribArray(2);       // texture
    RENDERER.VertexAttribPointer(2, 2, RD_FLOAT, RD_FALSE, VERTEX_SIZE * sizeof(float), (void*)(6 * sizeof(float)));

    RENDERER.BindVertexArray(0);
    RENDERER.BindBuffer(RD_ARRAY_BUFFER, 0);
    RENDERER.BindBuffer(RD_ELEMENT_ARRAY_BUFFER, 0);

    RENDERER.DisableVertexAttribArray(0);
    RENDERER.DisableVertexAttribArray(1);
    RENDERER.DisableVertexAttribArray(2);
}