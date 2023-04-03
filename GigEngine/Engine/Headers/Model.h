#pragma once
#include "IResource.h"
#include "Component.h"
#include "Mesh.h"
#include <assimp/scene.h>

struct Material 
{

};

class Model : public IResource
{
public:
	Model(std::string const& filePath);
	~Model();

	void Draw() const;
	void SetTexture(const std::string& filePath);

private:
	void LoadModel(const std::string& pPath);
	void ProcessNode(const aiNode* pNode, const aiScene* pScene);
	void ProcessMesh(const aiMesh* pMesh, const aiScene* pScene);
	void ProcessMaterial(const aiScene* pScene);

	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
};
