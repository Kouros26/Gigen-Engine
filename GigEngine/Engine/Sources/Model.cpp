#include "Model.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "ResourceManager.h"

Model::Model(std::string const& pFilePath)
	:IResource(pFilePath)
{
	LoadModel(pFilePath);
	texture = ResourceManager::Get<Texture>(g_defaultTexturePath);
}

Model::~Model()
{
	for (const auto& mesh : meshes)
	{
		delete mesh;
	}
	for (const auto& mat : materials)
	{
		delete mat;
	}
}

void Model::Draw() const
{
	texture->Bind();
	for (int i = 0; i < meshes.size(); i++)
	{
		if (meshes[i])
		{
			if (meshes[i]->materialIndex < materials.size())
			{
				if (materials[meshes[i]->materialIndex])
				{
					materials[meshes[i]->materialIndex]->SendToShader();
				}
				else
				{
					Material::SendDefaultMaterial();
				}
			}
			meshes[i]->Draw();
		}
	}
	Texture::UnBind();
}

void Model::SetTexture(const std::string& pFilePath)
{
	texture = ResourceManager::Get<Texture>(pFilePath);
	if (!texture->isValid())
	{
		std::cout << "texture invalid for model" << std::endl;
		texture = ResourceManager::Get<Texture>(g_defaultTexturePath);
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

		Material* m = new Material();

		//diffuse & ambient are inverted ??
		aiColor4D vec4;
		if (AI_SUCCESS == aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &vec4))
		{
			m->SetAmbient(lm::FVec3(vec4.r, vec4.g, vec4.b));
		}
		if (AI_SUCCESS == aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_AMBIENT, &vec4))
		{
			m->SetDiffuse(lm::FVec3(vec4.r, vec4.g, vec4.b));
		}
		if (AI_SUCCESS == aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_SPECULAR, &vec4))
		{
			m->SetSpecular(lm::FVec3(vec4.r, vec4.g, vec4.b));
		}

		unsigned int max;
		float shininess;
		aiGetMaterialFloatArray(pMaterial, AI_MATKEY_SHININESS, &shininess, &max);

		m->SetShininess(shininess);

		materials.push_back(m);
	}
}