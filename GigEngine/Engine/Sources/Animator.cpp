#include "Animator.h"
#include "Application.h"

AnimationState::AnimationState(Animation* pAnimation, const std::string& pStateName, AnimationState* pParentState,
	float ptimeToTransitionToThisState)
		: stateName(pStateName), stateAnim(pAnimation), parent(pParentState), timeToTransitionToThisState(ptimeToTransitionToThisState)
{
	if (parent)
		parent->children.push_back(*this);
}

Animator::Animator(GameObject* pOwner)
	: Component(pOwner), rootState(nullptr, "Idle", nullptr, 3), currentState(rootState), currentTime(0)
{
	finalBoneMatrices.reserve(g_max_bones);

	for (int i = 0; i < g_max_bones; i++)
		finalBoneMatrices.emplace_back(1.0f);
}

void Animator::Start()
{
	currentState.stateAnim = rootState.stateAnim;
	currentTime = 0.0;
}

void Animator::Update(float pDeltaTime)
{
	if (currentState.stateAnim)
	{
		currentTime += currentState.stateAnim->GetTicksPerSecond() * pDeltaTime;
		currentTime = fmod(currentTime, currentState.stateAnim->GetDuration());
		CalculateBoneTransform(&currentState.stateAnim->GetRootNode(), lm::FMat4(1.0f));

		if (isTransitioning)
		{
			transitionTime += pDeltaTime;
			CalculateBoneTransform(&targetedState->stateAnim->GetRootNode(), lm::FMat4(1.0f));
		}
	}

	for (unsigned int i = 0; i < finalBoneMatrices.size(); i++)
	{
		std::string s = "finalBonesMatrices[" + std::to_string(i) + "]";
		Application::GetMainShader().SetMat4(finalBoneMatrices[i], s.c_str());
	}
}

Component* Animator::Clone(GameObject* newGameObject)
{
	return new Animator(newGameObject);
}

void Animator::StateChange(const std::string& pNewStateName)
{
	for (auto& child : currentState.children)
		if (child.stateName == pNewStateName)
		{
			targetedState = &child;
			isTransitioning = true;
			return;
		}

	if (currentState.parent->stateName == pNewStateName)
	{
		targetedState = currentState.parent;
		isTransitioning = true;
	}
}

AnimationState& Animator::GetAnimationStateRoot()
{
	return rootState;
}

void Animator::CalculateBoneTransform(const NodeData* pNode, const lm::FMat4& pParentTransform)
{
	const std::string pNodeName = pNode->name;
	lm::FMat4 pNodeTransform = pNode->transform;

	if (Bone* bone = currentState.stateAnim->FindBone(pNodeName))
	{
		bone->Update(currentTime);
		pNodeTransform = bone->GetLocalTransform();
	}

	const lm::FMat4 globalTransformation = pParentTransform * pNodeTransform;

	auto boneInfoMap = currentState.stateAnim->GetBoneIDMap();
	if (boneInfoMap.contains(pNodeName))
	{
		const int index = boneInfoMap[pNodeName].id;
		const lm::FMat4 offset = boneInfoMap[pNodeName].offset;
		finalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < pNode->childrenCount; i++)
		CalculateBoneTransform(&pNode->children[i], globalTransformation);
}

std::vector<lm::FMat4>& Animator::GetFinalBoneMatrices()
{
	return finalBoneMatrices;
}