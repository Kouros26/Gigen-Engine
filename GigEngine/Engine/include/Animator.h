#pragma once
#include "Animation.h"

constexpr unsigned int g_max_bones = 100;

class Animator
{
private:
    std::vector<lm::FMat4> finalBoneMatrices;
    Animation* currentAnimation;
    float currentTime;

public:

    Animator(Animation* pAnimation);

    void PlayAnimation(Animation* pAnimation);
    void UpdateAnimation(float pDeltaTime);
    void CalculateBoneTransform(const NodeData* pNode, const lm::FMat4& pParentTransform);

    std::vector<lm::FMat4> GetFinalBoneMatrices();
};
