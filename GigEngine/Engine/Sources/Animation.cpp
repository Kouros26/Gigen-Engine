#include "Animation.h"
#include <cassert>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "ModelLoader.h"
#include "Mat3/FMat3.hpp"

Bone::Bone(const std::string& pName, int pId, const aiNodeAnim* pChannel)
	: name(pName), id(pId)
{
    numPositions = pChannel->mNumPositionKeys;

    for (int positionIndex = 0; positionIndex < numPositions; ++positionIndex)
    {
        aiVector3D aiPosition = pChannel->mPositionKeys[positionIndex].mValue;
        const float timeStamp = pChannel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        data.position = ModelLoader::AIVec3ToFVec3(aiPosition);
        data.timeStamp = timeStamp;
        positions.push_back(data);
    }

    numRotations = pChannel->mNumRotationKeys;
    for (int rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = pChannel->mRotationKeys[rotationIndex].mValue;
        const float timeStamp = pChannel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data;
        data.orientation = ModelLoader::AIQuatToFQuat(aiOrientation);
        data.timeStamp = timeStamp;
        rotations.push_back(data);
    }

    numScalings = pChannel->mNumScalingKeys;
    for (int keyIndex = 0; keyIndex < numScalings; ++keyIndex)
    {
        aiVector3D scale = pChannel->mScalingKeys[keyIndex].mValue;
        const float timeStamp = pChannel->mScalingKeys[keyIndex].mTime;
        KeyScale data;
        data.scale = ModelLoader::AIVec3ToFVec3(scale);
        data.timeStamp = timeStamp;
        scales.push_back(data);
    }
}

void Bone::Update(float pAnimationTime)
{
	localPos = InterpolatePosition(pAnimationTime);
	localRot = InterpolateRotation(pAnimationTime);
	localScl = InterpolateScaling(pAnimationTime);
}

int Bone::GetPositionIndex(float pAnimationTime) const
{
    for (int index = 0; index < numPositions - 1; ++index)
    {
        if (pAnimationTime < positions[index + 1].timeStamp)
            return index;
    }
    assert(0);
    return 0;
}

int Bone::GetRotationIndex(float pAnimationTime) const
{
    for (int index = 0; index < numRotations - 1; ++index)
    {
        if (pAnimationTime < rotations[index + 1].timeStamp)
            return index;
    }
    assert(0);
    return 0;
}

int Bone::GetScaleIndex(float pAnimationTime) const
{
    for (int index = 0; index < numScalings - 1; ++index)
    {
        if (pAnimationTime < scales[index + 1].timeStamp)
            return index;
    }
    assert(0);
    return 0;
}

lm::FMat4 Bone::GetLocalTransform() const
{
    return AnimUtils::GetTransform(localPos, localRot, localScl);
}

lm::FVec3& Bone::GetLocalPos()
{
    return localPos;
}

lm::FQuat& Bone::GetLocalRot()
{
    return localRot;
}

lm::FVec3& Bone::GetLocalScl()
{
    return localScl;
}

std::string Bone::GetBoneName() const
{
    return name;
}

int Bone::GetBoneID() const
{
    return id;
}

float Bone::GetScaleFactor(float pLastTimeStamp, float pNextTimeStamp, float pAnimationTime)
{
    float scaleFactor = 0.0f;
    const float midWayLength = pAnimationTime - pLastTimeStamp;
    const float framesDiff = pNextTimeStamp - pLastTimeStamp;
    scaleFactor = midWayLength / framesDiff;

    return scaleFactor;
}

lm::FVec3 Bone::InterpolatePosition(float pAnimationTime)
{
    if (numPositions == 1)
        return positions[0].position;

    const int p0Index = GetPositionIndex(pAnimationTime);
    const int p1Index = p0Index + 1;
    const float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp,
                                             positions[p1Index].timeStamp, pAnimationTime);
    return lm::FVec3::Lerp(positions[p0Index].position,positions[p1Index].position, scaleFactor);
}

lm::FQuat Bone::InterpolateRotation(float pAnimationTime)
{
    if (numRotations == 1)
        return lm::FQuat::Normalize(rotations[0].orientation);

    const int p0Index = GetRotationIndex(pAnimationTime);
    const int p1Index = p0Index + 1;
    const float scaleFactor = GetScaleFactor(rotations[p0Index].timeStamp,
                                             rotations[p1Index].timeStamp, pAnimationTime);
    const lm::FQuat finalRotation = lm::FQuat::SLerp(rotations[p0Index].orientation,
                                                     rotations[p1Index].orientation, scaleFactor);

    return lm::FQuat::Normalize(finalRotation);
}

lm::FVec3 Bone::InterpolateScaling(float pAnimationTime)
{
    if (numScalings == 1)
        return scales[0].scale;

    const int p0Index = GetScaleIndex(pAnimationTime);
    const int p1Index = p0Index + 1;
    const float scaleFactor = GetScaleFactor(scales[p0Index].timeStamp,
                                             scales[p1Index].timeStamp, pAnimationTime);

    return lm::FVec3::Lerp(scales[p0Index].scale, scales[p1Index].scale, scaleFactor);
}

Animation::Animation(const std::string& pAnimationPath, Model* pModel)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(pAnimationPath, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    const auto animation = scene->mAnimations[0];
    duration = animation->mDuration;
    ticksPerSecond = animation->mTicksPerSecond;
    ReadHierarchyData(rootNode, scene->mRootNode->mChildren[0]);
    ReadMissingBones(animation, *pModel);
}

Animation::~Animation()
= default;

void Animation::Init()
{
}

Bone* Animation::FindBone(const std::string& pName)
{
	const auto it = std::ranges::find_if(bones,[&](const Bone& Bone){return Bone.GetBoneName() == pName;}
	);
    if (it == bones.end()) 
        return nullptr;

	return &(*it);
}

double Animation::GetTicksPerSecond() const
{
    return ticksPerSecond;
}

double Animation::GetDuration() const
{
    return duration;
}

NodeData& Animation::GetRootNode()
{
    return rootNode;
}

std::map<std::string, BoneInfo>& Animation::GetBoneIDMap()
{
    return boneMap;
}

std::vector<Bone>& Animation::GetBones()
{
    return bones;
}

void Animation::ReadMissingBones(const aiAnimation* pAnimation, Model& pModel)
{
	const unsigned int size = pAnimation->mNumChannels;

    auto& boneInfoMap = pModel.GetBoneInfoMap();
    int& boneCount = pModel.GetBoneCount();

    //reading channels(bones engaged in an animation and their keyframes)
    for (unsigned int i = 0; i < size; i++)
    {
	    const auto channel = pAnimation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (!boneInfoMap.contains(boneName))
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        bones.emplace_back(channel->mNodeName.data,
                             boneInfoMap[channel->mNodeName.data].id, channel);
    }

    boneMap = boneInfoMap;
}

void Animation::ReadHierarchyData(NodeData& pOutData, const aiNode* pNode)
{
    assert(pNode);

    pOutData.name = pNode->mName.data;
    pOutData.transform = ModelLoader::AIMat4toFMat4(pNode->mTransformation);
    pOutData.childrenCount = pNode->mNumChildren;

    for (unsigned int i = 0; i < pNode->mNumChildren; i++)
    {
        NodeData newData;
        ReadHierarchyData(newData, pNode->mChildren[i]);
        pOutData.children.push_back(newData);
    }
}

lm::FMat4 AnimUtils::GetTransform(const lm::FVec3& pPos, const lm::FQuat& pRot, const lm::FVec3& pScl)
{
	lm::FMat4 bob = lm::FMat4::Translation(pPos) * lm::FMat4(lm::FQuat::ToRotateMat3(pRot)) * lm::FMat4::Scale({1.0f},
		pScl);
    return bob;
}
