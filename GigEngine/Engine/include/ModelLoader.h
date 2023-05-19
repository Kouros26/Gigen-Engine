#pragma once
#include "BoneInfo.h"
#include <assimp/matrix4x4.h>
#include <map>
#include <vector>
#include <string>
#include <assimp/quaternion.h>


class Mesh;
class Material;
struct aiNode;
struct aiScene;
struct aiMesh;

constexpr unsigned int VERTEX_SIZE = 16;
constexpr unsigned int FACE_SIZE = 3;

class ModelLoader
{
public:

    void LoadModel(std::vector<Mesh*>& meshes, std::vector<Material*>& materials, std::map<std::string, BoneInfo>& boneInfo, int& boneCounter, std::string const& pFilePath);

    static lm::FMat4 AIMat4toFMat4(const aiMatrix4x4& pMatrix);
    static lm::FVec3 AIVec3ToFVec3(const aiVector3D& pVector);
    static lm::FQuat AIQuatToFQuat(const aiQuaternion& pQuaternion);

private:
    void ProcessNode(const aiNode* pNode, const aiScene* pScene, std::vector<Mesh*>& meshes, std::vector<Material*>& materials, std::map<std::string, BoneInfo>& boneInfo, int& boneCounter);
    void ProcessMesh(const aiMesh* pMesh, const aiScene* pScene, std::vector<Mesh*>& meshes, std::map<std::string, BoneInfo>& boneInfo, int& boneCounter);
    void ProcessBones(const aiMesh* pMesh, const aiScene* pScene, Mesh* mesh, std::map<std::string, BoneInfo>& boneInfo, int& boneCounter);
    void ProcessMaterial(const aiScene* pScene, std::vector<Material*>& materials);

    void SetVertexBoneDataDefault(const Mesh* pMesh, int pVertexId);
    void SetVertexBoneData(const Mesh* pMesh, int pVertexId, int pId, float pWeight); //Not sure if it works
};
