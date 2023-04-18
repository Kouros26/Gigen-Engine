#pragma once
#include "IResource.h"
#include "ModelLoader.h"
#include <vector>

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
    void Draw() const;
    void SetTexture(const std::string& pFilePath);

    void Init() override;

private:

    std::vector<Mesh*> meshes;
    std::vector<Material*> materials;
    Texture* texture = nullptr;
};
