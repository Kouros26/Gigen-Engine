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

	void Draw() const;
	void SetTexture(const std::string& pFilePath);

private:
	void LoadModel(const std::string& pPath);
	void ProcessNode(const aiNode* pNode, const aiScene* pScene);
	void ProcessMesh(const aiMesh* pMesh, const aiScene* pScene);
	void ProcessMaterial(const aiScene* pScene);

	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	Texture* texture = nullptr;
};
