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

    virtual Component* Clone(GameObject* newGameObject) = 0;

    Component(const Component& other) = default;
    Component(Component&& other) noexcept = default;
    Component& operator=(const Component& other) = default;
    Component& operator=(Component&& other) noexcept = default;

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

    virtual Component* Clone(GameObject* newGameObject) override {
        return new TestComponent(newGameObject);
    };
};

class testComponent2 : public Component
{
public:
    testComponent2(GameObject* gameObject) : Component(gameObject) {};

    virtual void Update() override {
        gameObject->GetTransform().AddRotation(lm::FVec3(0, 0, 20.f) * Time::GetDeltaTime());
    };

    virtual Component* Clone(GameObject* newGameObject) override {
        return new testComponent2(newGameObject);
    };
};