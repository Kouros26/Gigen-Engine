#pragma once
#include <map>
#include <vector>
#include <assimp/anim.h>

#include "BoneInfo.h"
#include "IResource.h"
#include "Model.h"
#include "Mat4/FMat4.hpp"
#include "Quaternion/FQuat.hpp"
#include "Vec3/FVec3.hpp"

struct KeyPosition
{
    lm::FVec3 position;
    float timeStamp;
};

struct KeyRotation
{
    lm::FQuat orientation;
    float timeStamp;
};

struct KeyScale
{
    lm::FVec3 scale;
    float timeStamp;
};

struct NodeData
{
    lm::FMat4 transform;
    std::string name;
    int childrenCount;
    std::vector<NodeData> children;
};

class Bone
{
private:
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
    int numPositions;
    int numRotations;
    int numScalings;

    lm::FMat4 localTransform;
    std::string name;
    int id;

public:

    Bone(const std::string& pName, int pId, const aiNodeAnim* pChannel);

    void Update(float pAnimationTime);

    [[nodiscard]] int GetPositionIndex(float pAnimationTime) const;
    [[nodiscard]] int GetRotationIndex(float pAnimationTime) const;
    [[nodiscard]] int GetScaleIndex(float pAnimationTime) const;

    [[nodiscard]] lm::FMat4 GetLocalTransform() const;
    [[nodiscard]] std::string GetBoneName() const;
    [[nodiscard]] int GetBoneID() const;

private:

    float GetScaleFactor(float pLastTimeStamp, float pNextTimeStamp, float pAnimationTime);

    lm::FMat4 InterpolatePosition(float pAnimationTime);
    lm::FMat4 InterpolateRotation(float pAnimationTime);
    lm::FMat4 InterpolateScaling(float pAnimationTime);
};

class Animation : public IResource
{
private:
    float duration;
    int ticksPerSecond;
    std::vector<Bone> bones;
    NodeData rootNode;
    std::map<std::string, BoneInfo> boneMap;

public:

    Animation() = default;
    Animation(const std::string& pAnimationPath, Model* pModel);
    ~Animation() override;

    Bone* FindBone(const std::string& pName);

    [[nodiscard]] float GetTicksPerSecond() const;
    [[nodiscard]] float GetDuration() const;
    NodeData& GetRootNode();
    std::map<std::string, BoneInfo>& GetBoneIDMap();
    std::vector<Bone>& GetBones();

private:

    void ReadMissingBones(const aiAnimation* pAnimation, Model& pModel);
    void ReadHierarchyData(NodeData& pOutData, const aiNode* pNode);
};
