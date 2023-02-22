#pragma once
#include "Component.h"
#include "Mesh.h"
#include "assimp/scene.h"

class Model : public Component
{
public:
	Model(GameObject* gameObject, std::string filePath);
	~Model();

	virtual void Update() override;

private:
	//should be done in resource manager
	void loadModel(const std::string& pPath);
	void processNode(const aiNode* pNode, const aiScene* pScene);
	void processMesh(const aiMesh* pMesh, const aiScene* pScene);

	//this should be a resource in resoure manager
	std::vector<Mesh*> meshes;
};
