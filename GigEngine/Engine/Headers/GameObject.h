#pragma once
#include <list>

#include "Transform.h"
#include <vector>

class Model;
class GameObject
{
public:
    GameObject();
    GameObject(const std::string& name);
    GameObject(const std::string& name, const lm::FVec3& position, const lm::FVec3& rotation, const lm::FVec3& scale);
    GameObject(const lm::FVec3& position, const lm::FVec3& rotation, const lm::FVec3& scale);

    GameObject(const GameObject& other);
    GameObject(GameObject&& other) noexcept = delete;
    GameObject& operator=(const GameObject& other);
    GameObject& operator=(GameObject&& other) noexcept = delete;

    virtual ~GameObject();

    void Destroy();

    void UpdateRender() const;
    void UpdateComponents() const;
    void UpdateHierarchy();

    std::string GetName();
    unsigned int GetId();

    void SetModel(const std::string& filePath);

    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);

    void AddComponent(class Component* newComponent);

    //create New component of type and return the new Component
    template<class T>
    T* AddComponent();

    //return first component of type
    template<class T>
    T* GetComponent();

    //return vector of all components of type
    template<class T>
    std::vector<T*> GetComponents();

    //remove all components of type
    template<class T>
    void RemoveComponents();

    Transform& GetTransform();

private:

    std::string name;
    unsigned int id;

    Transform transform;

    GameObject* parent = nullptr;
    std::list<GameObject*> children{};

    std::vector<Component*> components;
    Model* model = nullptr;

    //use so every gameObject has a different id
    static unsigned int gameObjectIndex;
};

template<class T>
inline T* GameObject::AddComponent() {
    T* newComp = new T(this);
    components.push_back(newComp);
    return newComp;
}

template<class T>
inline T* GameObject::GetComponent()
{
    for (int i = 0; i < components.size(); i++)
    {
        T* comp = dynamic_cast<T*>(components[i]);
        if (comp)
        {
            return comp;
        }
    }
    return nullptr;
}

template<class T>
inline std::vector<T*> GameObject::GetComponents()
{
    std::vector<T*> comps;
    for (int i = 0; i < components.size(); i++)
    {
        if (T* comp = dynamic_cast<T*>(components[i]))
            comps.push_back(comp);
    }
    return comps;
}

template<class T>
inline void GameObject::RemoveComponents()
{
    for (int i = 0; i < components.size(); i++)
    {
        if (const T* comp = dynamic_cast<T*>(components[i]))
        {
            delete comp;
            components.erase(components.begin() + i);
        }
    }
}