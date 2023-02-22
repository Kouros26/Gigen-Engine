#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

Model::Model(GameObject* gameObject, std::string filePath)
	:Component(gameObject)
{
	loadModel(filePath);
}

Model::~Model()
{
	//to remove into ressource manager
	for (const auto& mesh : meshes)
		delete mesh;
}

void Model::Update()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw();
	}
}

void Model::loadModel(const std::string& pPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pPath, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "error loading assimp scene" << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode* pNode, const aiScene* pScene)
{
	for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
	{
		const aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
		processMesh(mesh, pScene);
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
		processNode(pNode->mChildren[i], pScene);
}

void Model::processMesh(const aiMesh* pMesh, const aiScene* pScene)
{
	Mesh* mesh = new Mesh(pMesh->mNumVertices * 8, pMesh->mNumFaces * 3);
	mesh->vertices = new float[pMesh->mNumVertices * 8]; //8 is 3 pos + 3 normal + 2 texture

	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		mesh->vertices[i * 8] = pMesh->mVertices[i].x;
		mesh->vertices[(i * 8) + 1] = pMesh->mVertices[i].y;
		mesh->vertices[(i * 8) + 2] = pMesh->mVertices[i].z;

		if (pMesh->HasNormals())
		{
			mesh->vertices[(i * 8) + 3] = pMesh->mNormals[i].x;
			mesh->vertices[(i * 8) + 4] = pMesh->mNormals[i].y;
			mesh->vertices[(i * 8) + 5] = pMesh->mNormals[i].z;
		}

		if (pMesh->mTextureCoords[0])
		{
			mesh->vertices[(i * 8) + 6] = pMesh->mTextureCoords[0][i].x;
			mesh->vertices[(i * 8) + 7] = pMesh->mTextureCoords[0][i].y;
		}
	}

	mesh->indices = new unsigned int[pMesh->mNumFaces * 3]; // *3 because triangle

	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	{
		mesh->indices[(i * 3)] = pMesh->mFaces[i].mIndices[0];
		mesh->indices[(i * 3) + 1] = pMesh->mFaces[i].mIndices[1];
		mesh->indices[(i * 3) + 2] = pMesh->mFaces[i].mIndices[2];
	}

	mesh->setUpBuffers();
	meshes.emplace_back(mesh);
}