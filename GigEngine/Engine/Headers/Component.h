#pragma once
#include "Watch.h"
#include "GameObject.h"

class Component
{
public:
	Component(GameObject* gameObject);
	virtual ~Component();

	virtual void Update();

protected:
	GameObject* gameObject;
};

class testComponent : public Component
{
public:
	testComponent(GameObject* gameObject) : Component(gameObject) {};

	virtual void Update() override {
		gameObject->GetTransform().AddRotation(lm::FVec3(0, 0, 0.2f) * Time::GetDeltaTime());
	};
};
