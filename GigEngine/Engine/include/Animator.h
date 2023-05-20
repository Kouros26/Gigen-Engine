#pragma once
#include "Animation.h"
#include "Component.h"

constexpr unsigned int g_max_bones = 100;

struct AnimationState
{
    std::string stateName;

    Animation* stateAnim;

    AnimationState* parent;
    std::vector<AnimationState> children;

    float timeToTransitionToThisState;

    AnimationState(Animation* pAnimation, const std::string& pStateName, AnimationState* pParentState, float ptimeToTransitionToThisState);
};

class Animator : public Component
{
private:
    std::vector<lm::FMat4> finalBoneMatrices;
    AnimationState rootState;
    AnimationState& currentState;
    AnimationState* targetedState;
    float currentTime;
    float transitionTime;
    bool isTransitioning = false;

public:

    Animator(GameObject* pOwner);

    void Start() override;

    void Update(float pDeltaTime) override;

	Component* Clone(GameObject* newGameObject) override;

    void StateChange(const std::string& pNewStateName);

    AnimationState& GetAnimationStateRoot();
    std::vector<lm::FMat4>& GetFinalBoneMatrices();

private:

	void CalculateBoneTransform(const NodeData* pNode, const lm::FMat4& pParentTransform);
};
