#include "Animator.h"

#include "Application.h"

Animator::Animator(Animation* pAnimation)
	: currentAnimation(nullptr), currentTime(0)
{
	if (pAnimation)
	{
		finalBoneMatrices.reserve(pAnimation->GetBones().size());

		for (int i = 0; i < finalBoneMatrices.capacity(); i++)
			finalBoneMatrices.emplace_back(1.0f);
	}

	else
	{
		finalBoneMatrices.reserve(g_max_bones);

		for (int i = 0; i < g_max_bones; i++)
			finalBoneMatrices.emplace_back(1.0f);
	}
}

void Animator::PlayAnimation(Animation* pAnimation)
{
	currentAnimation = pAnimation;
	currentTime = 0.0f;
}

void Animator::UpdateAnimation(float pDeltaTime)
{
	if (currentAnimation)
	{
		currentTime += currentAnimation->GetTicksPerSecond() * pDeltaTime;
		currentTime = fmod(currentTime, currentAnimation->GetDuration());
		CalculateBoneTransform(&currentAnimation->GetRootNode(), lm::FMat4(1.0f));
	}

	for (int i = 0; i < finalBoneMatrices.size(); i++)
	{
		std::string s = "finalBonesMatrices[" + std::to_string(i) + "]";
		Application::GetMainShader().SetMat4(finalBoneMatrices[i], s.c_str());
	}
}

void Animator::CalculateBoneTransform(const NodeData* pNode, const lm::FMat4& pParentTransform)
{
	const std::string pNodeName = pNode->name;
	lm::FMat4 pNodeTransform = pNode->transform;

	if (Bone* bone = currentAnimation->FindBone(pNodeName))
	{
		bone->Update(currentTime);
		pNodeTransform = bone->GetLocalTransform();
	}
	else
	{
		std::cout << "bone not found : " << pNodeName << std::endl;
	}

	const lm::FMat4 globalTransformation = pParentTransform * pNodeTransform;

	auto boneInfoMap = currentAnimation->GetBoneIDMap();
	if (boneInfoMap.contains(pNodeName))
	{
		const int index = boneInfoMap[pNodeName].id;
		const lm::FMat4 offset = boneInfoMap[pNodeName].offset;
		finalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < pNode->childrenCount; i++)
		CalculateBoneTransform(&pNode->children[i], globalTransformation);
}

std::vector<lm::FMat4> Animator::GetFinalBoneMatrices()
{
	return finalBoneMatrices;
}
