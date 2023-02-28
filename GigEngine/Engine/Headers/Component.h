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

class TestComponent : public Component
{
public:
	TestComponent(GameObject* gameObject) : Component(gameObject) {};

	virtual void Update() override {
		gameObject->transform.SetPosition(lm::FVec3(1) * sin(Time::GetCurrentTime()));
		gameObject->transform.AddRotation(lm::FVec3(0,10,0) * Time::GetDeltaTime());
	};
};
