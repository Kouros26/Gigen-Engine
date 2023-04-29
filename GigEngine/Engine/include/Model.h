#pragma once
#include "IResource.h"
#include "ModelLoader.h"
#include <vector>

const char* const g_defaultModelPath = "Resources/Models/Basics/Cube.FBX";

class Texture;
class Material;
class Mesh;

class Model : public IResource, public ModelLoader
{
public:
    Model(std::string const& pFilePath);
    ~Model();

    Model(const Model& other);
    Model(Model&& other) noexcept;
    Model& operator=(const Model& other);
    Model& operator=(Model&& other) noexcept;
    void Draw(Texture* texture) const;

    void Init() override;

private:

    std::vector<Mesh*> meshes;
    std::vector<Material*> materials;
};
