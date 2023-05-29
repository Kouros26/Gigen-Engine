#include "Animator.h"
#include "Application.h"
#include "Mat3/FMat3.hpp"

AnimationState::AnimationState(Animation* pAnimation, const std::string& pStateName, AnimationState* pParentState,
                               float ptimeToTransitionToThisState)
		: stateName(pStateName), stateAnim(pAnimation), parent(pParentState), timeToTransitionToThisState(ptimeToTransitionToThisState)
{
	if (parent)
		parent->children.push_back(*this);
}

Animator::Animator(GameObject* pOwner)
	: Component(pOwner), rootState(nullptr, "Idle", nullptr, 3), currentState(&rootState)
{
	finalBoneMatrices.reserve(g_max_bones);

	for (int i = 0; i < g_max_bones; i++)
		finalBoneMatrices.emplace_back(1.0f);
}

void Animator::Start()
{
	currentState->stateAnim = rootState.stateAnim;
	currentTime = 0.0;
}

void Animator::Update(float pDeltaTime)
{
	if (currentState->stateAnim)
	{
		currentTime += currentState->stateAnim->GetTicksPerSecond() * pDeltaTime;
		currentTime = fmod(currentTime, currentState->stateAnim->GetDuration());

		if (isTransitioning)
		{
			transitionTimer += pDeltaTime;
			transitionTime += targetedState->stateAnim->GetTicksPerSecond() * pDeltaTime;
			transitionTime = fmod(transitionTime, targetedState->stateAnim->GetDuration());

			if (transitionTimer >= targetedState->timeToTransitionToThisState)
			{
				currentTime = fmod(transitionTimer * targetedState->stateAnim->GetTicksPerSecond(), targetedState->stateAnim->GetDuration());
				std::cout << currentTime << std::endl;
				transitionTime = 0;
				transitionTimer = 0;
				isTransitioning = false;
				currentState = targetedState;
				targetedState = nullptr;
			}
		}
		CalculateBoneTransform(&currentState->stateAnim->GetRootNode(), lm::FMat4(1.0f));
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
	for (auto& child : currentState->children)
		if (child.stateName == pNewStateName)
		{
			targetedState = &child;
			isTransitioning = true;
			return;
		}

	if (!currentState->parent)
		return;

	if (currentState->parent->stateName == pNewStateName)
	{
		targetedState = currentState->parent;
		isTransitioning = true;
	}
}

AnimationState& Animator::GetAnimationStateRoot()
{
	return rootState;
}

AnimationState* Animator::GetCurrentState() const
{
	return currentState;
}

void Animator::CalculateBoneTransform(const NodeData* pNode, const lm::FMat4& pParentTransform)
{
	currentStateNode.nodeName = pNode->name;
	currentStateNode.transform = pNode->transform;

	if (Bone* bone = currentState->stateAnim->FindBone(currentStateNode.nodeName))
	{
		isBoneAnimated = true;
		bone->Update(currentTime);
		currentStateNode.localPos = bone->GetLocalPos();
		currentStateNode.localRot = bone->GetLocalRot();
		currentStateNode.localScl = bone->GetLocalScl();
	}

	if (targetedState)
	{
		if (Bone* bone = targetedState->stateAnim->FindBone(currentStateNode.nodeName))
		{
			bone->Update(transitionTime);
			targetedStateNode.localPos = bone->GetLocalPos();
			targetedStateNode.localRot = bone->GetLocalRot();
			targetedStateNode.localScl = bone->GetLocalScl();

			PerformCrossFade();
		}
	}

	lm::FMat4 globalTransformation;

	if (isBoneAnimated)
	{
		globalTransformation = pParentTransform * AnimUtils::GetTransform(currentStateNode.localPos, currentStateNode.localRot, currentStateNode.localScl);
		isBoneAnimated = false;
	}

	else
		globalTransformation = pParentTransform * currentStateNode.transform;

	auto boneInfoMap = currentState->stateAnim->GetBoneIDMap();
	if (boneInfoMap.contains(currentStateNode.nodeName))
	{
		const int index = boneInfoMap[currentStateNode.nodeName].id;
		const lm::FMat4 offset = boneInfoMap[currentStateNode.nodeName].offset;
		finalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < pNode->childrenCount; i++)
		CalculateBoneTransform(&pNode->children[i], globalTransformation);
}

void Animator::PerformCrossFade()
{
	currentStateNode.localPos = lm::FVec3::Lerp(currentStateNode.localPos, targetedStateNode.localPos,
		transitionTimer / targetedState->timeToTransitionToThisState);
	currentStateNode.localRot = lm::FQuat::SLerp(currentStateNode.localRot, targetedStateNode.localRot,
					transitionTimer / targetedState->timeToTransitionToThisState);
	currentStateNode.localScl = lm::FVec3::Lerp(currentStateNode.localScl, targetedStateNode.localScl,
		transitionTimer / targetedState->timeToTransitionToThisState);
}

std::vector<lm::FMat4>& Animator::GetFinalBoneMatrices()
{
	return finalBoneMatrices;
}