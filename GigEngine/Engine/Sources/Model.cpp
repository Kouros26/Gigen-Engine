#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "ResourceManager.h"
#include "Texture.h"

Model::Model(std::string const& filePath)
	:IResource(filePath)
{
	LoadModel(filePath);
}

Model::~Model()
{
	for (const auto& mesh : meshes) 
	{
		delete mesh;
	}
}

void Model::Draw() const
{
	for (int i = 0; i < meshes.size(); i++) 
	{
		if (meshes[i]) 
		{
			Texture* t = ResourceManager::Get<Texture>(meshes[i]->texturePath);
			if (t)
			{
				t->Bind();
			}
			meshes[i]->Draw();
			//glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void Model::SetTexture(const std::string& filePath)
{
	for (const auto& mesh : meshes)
	{
		mesh->texturePath = filePath;
	}
}

void Model::LoadModel(const std::string& pPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pPath, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "error loading assimp scene" << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(const aiNode* pNode, const aiScene* pScene)
{
	for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
	{
		const aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
		ProcessMesh(mesh, pScene);
	}

	ProcessMaterial(pScene);

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
		ProcessNode(pNode->mChildren[i], pScene);
}

void Model::ProcessMesh(const aiMesh* pMesh, const aiScene* pScene)
{
	Mesh* mesh = new Mesh(pMesh->mNumVertices * VERTEX_SIZE, pMesh->mNumFaces * FACE_SIZE);
	mesh->vertices = new float[pMesh->mNumVertices * VERTEX_SIZE];
	mesh->materialIndex = pMesh->mMaterialIndex;

	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		mesh->vertices[i * VERTEX_SIZE] = pMesh->mVertices[i].x;
		mesh->vertices[(i * VERTEX_SIZE) + 1] = pMesh->mVertices[i].y;
		mesh->vertices[(i * VERTEX_SIZE) + 2] = pMesh->mVertices[i].z;

		if (pMesh->HasNormals())
		{
			mesh->vertices[(i * VERTEX_SIZE) + 3] = pMesh->mNormals[i].x;
			mesh->vertices[(i * VERTEX_SIZE) + 4] = pMesh->mNormals[i].y;
			mesh->vertices[(i * VERTEX_SIZE) + 5] = pMesh->mNormals[i].z;
		}

		if (pMesh->mTextureCoords[0])
		{
			mesh->vertices[(i * VERTEX_SIZE) + 6] = pMesh->mTextureCoords[0][i].x;
			mesh->vertices[(i * VERTEX_SIZE) + 7] = pMesh->mTextureCoords[0][i].y;
		}
	}

	mesh->indices = new unsigned int[pMesh->mNumFaces * FACE_SIZE];

	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	{
		mesh->indices[(i * FACE_SIZE)] = pMesh->mFaces[i].mIndices[0];
		mesh->indices[(i * FACE_SIZE) + 1] = pMesh->mFaces[i].mIndices[1];
		mesh->indices[(i * FACE_SIZE) + 2] = pMesh->mFaces[i].mIndices[2];
	}

	mesh->setUpBuffers();
	meshes.emplace_back(mesh);
}

void Model::ProcessMaterial(const aiScene* pScene)
{
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		
	}
}
