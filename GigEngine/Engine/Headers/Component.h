#pragma once
#include "Watch.h"
#include "GameObject.h"

class Component
{
public:
	Component(GameObject* gameObject);
	virtual ~Component();

	virtual void Start();
	virtual void Update();

protected:
	GameObject* gameObject;
};

class TestComponent : public Component
{
public:
	TestComponent(GameObject* gameObject) : Component(gameObject) {};

	virtual void Update() override {
		gameObject->GetTransform().AddPosition(lm::FVec3(0, 0.5f, 0.f) * Time::GetDeltaTime());
	};
};

class testComponent2 : public Component
{
public:
	testComponent2(GameObject* gameObject) : Component(gameObject) {};

	virtual void Update() override {
		gameObject->GetTransform().AddRotation(lm::FVec3(0, 0, 20.f) * Time::GetDeltaTime());
	};
};