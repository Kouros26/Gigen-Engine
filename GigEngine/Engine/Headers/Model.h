#pragma once
#include "IResource.h"
#include <assimp/scene.h>
#include <vector>

class Texture;
class Material;
class Mesh;

class Model : public IResource
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
	void LoadModel(const std::string& pPath);
	void ProcessNode(const aiNode* pNode, const aiScene* pScene);
	void ProcessMesh(const aiMesh* pMesh, const aiScene* pScene);
	void ProcessMaterial(const aiScene* pScene);

	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	Texture* texture = nullptr;
};
