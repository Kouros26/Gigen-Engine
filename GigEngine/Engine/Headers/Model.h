#pragma once
#include "IResource.h"
#include "Component.h"
#include "Mesh.h"
#include <assimp/scene.h>

class Model : public IResource
{
public:
	Model(std::string const& filePath);
	~Model();

	void Draw();

private:
	void loadModel(const std::string& pPath);
	void processNode(const aiNode* pNode, const aiScene* pScene);
	void processMesh(const aiMesh* pMesh, const aiScene* pScene);

	std::vector<Mesh*> meshes;
};
