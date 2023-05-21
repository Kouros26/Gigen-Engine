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

struct HandledNode
{
    std::string nodeName;

    lm::FMat4 transform;

    lm::FVec3 localPos;
    lm::FQuat localRot;
    lm::FVec3 localScl;
};

class Animator : public Component
{
private:
    std::vector<lm::FMat4> finalBoneMatrices;
    AnimationState rootState;
    AnimationState* currentState;
    AnimationState* targetedState = nullptr;
    float currentTime = 0;
    float transitionTime = 0;
    float transitionTimer = 0;
    bool isTransitioning = false;

    HandledNode currentStateNode;
    HandledNode targetedStateNode;

public:

    Animator(GameObject* pOwner);

    void Start() override;

    void Update(float pDeltaTime) override;

	Component* Clone(GameObject* newGameObject) override;

    void StateChange(const std::string& pNewStateName);

    AnimationState& GetAnimationStateRoot();
    [[nodiscard]] AnimationState* GetCurrentState() const;
    std::vector<lm::FMat4>& GetFinalBoneMatrices();

private:

	void CalculateBoneTransform(const NodeData* pNode, const lm::FMat4& pParentTransform);
    void PerformCrossFade();
};
