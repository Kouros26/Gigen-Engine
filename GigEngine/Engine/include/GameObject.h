#pragma once
#include <list>
#include "Transform.h"
#include <vector>

namespace GigScripting
{
    class Behaviour;
}

struct Collision;
class Component;
class Model;
class Texture;

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

    void CreateBoxRigidBody(const lm::FVec3& halfExtents, const lm::FVec3& scale, float mass);
    void CreateCapsuleRigidBody(float radius, float height, const lm::FVec3& scale, float mass);
    void CreateSphereRigidBody(float radius, const lm::FVec3& scale, float mass);

    virtual void UpdateRender() const;
    void UpdateComponents() const;
    void UpdateHierarchy();

    void SetModel(const std::string& filePath);
    void SetModel(Model* pModel);
    void SetModelWithPathLua(const std::string& filePath);
    void SetTexture(const std::string& filePath);
    void SetTextureWithPathLua(const std::string& filePath);

    [[nodiscard]] Model* GetModel() const;
    [[nodiscard]] Texture* GetTexture() const;
    virtual std::string GetType();

    void LateUpdate() const;

    std::string GetName();
    void SetName(const std::string& pName);

    [[nodiscard]] unsigned int GetId() const;

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);

    virtual void OnCollisionEnter(const Collision& collision);
    virtual void OnCollisionExit(const Collision& collision);

    void AddComponent(Component* newComponent);

    //create new component of type and return the new component
    template<class T>
    T* AddComponent();
    template<class T, typename ...Args>
    T* AddComponent(Args...  pArgs);

    //return first component of type
    template<class T>
    T* GetComponent();

    [[nodiscard]] Component* GetComponentByID(int id) const;

    //return vector of all components of type
    template<class T>
    std::vector<T*>& GetComponents();
    template<class T>
    std::vector<T*>& GetComponents(std::vector<T*>& pVec);
    //remove all components of type
    template<class T>
    void RemoveComponents();

    [[nodiscard]] unsigned int GetComponentCount() const;

    Transform& GetTransform();
    [[nodiscard]] RigidBody* GetRigidBody() const;

	GameObject*& GetParent();
	void SetParent(GameObject* newParent);
	GameObject* GetChild(unsigned int index);
	unsigned int GetChildrenCount();
	std::list<GameObject*>& GetChildren();

	bool IsAParent(GameObject* obj) const;
	[[nodiscard]] bool IsActive() const;
	void SetActive(bool b);

    void CheckForScript(Component* pComponent);

	GigScripting::Behaviour* GetBehaviour(const std::string& pName);
	void RemoveScript(GigScripting::Behaviour* pScript);

    void AddScript(const std::string& path);
    void AddScript();

private:

    bool isActive;
    std::string name{};
    unsigned int id{};

    Transform transform{};
    RigidBody* rigidBody = nullptr;

    GameObject* parent = nullptr;
    std::list<GameObject*> children{};

    std::vector<Component*> components{};

    Model* model = nullptr;
    Texture* texture = nullptr;

    //use so every gameObject has a different id
    static unsigned int gameObjectIndex;
};

template<class T>
T* GameObject::AddComponent()
{
    T* newComp = new T(this);
    components.push_back(newComp);

    CheckForScript(newComp);

    return newComp;
}

template<class T, typename ...Args>
T* GameObject::AddComponent(Args... pArgs)
{
    T* newComp = new T(this, pArgs...);
    components.push_back(newComp);

    CheckForScript(newComp);

    return newComp;
}

template<class T>
T* GameObject::GetComponent()
{
    for (int i = 0; i < components.size(); i++)
    {
        if (T* comp = dynamic_cast<T*>(components[i]))
            return comp;
    }
    return nullptr;
}

inline Component* GameObject::GetComponentByID(const int componentID) const
{
    return components[componentID];
}

template<class T>
std::vector<T*>& GameObject::GetComponents()
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
std::vector<T*>
& GameObject::GetComponents(std::vector<T*>& pVec)
{
    for (int i = 0; i < components.size(); i++)
    {
        if (T* comp = dynamic_cast<T*>(components[i]))
            pVec.push_back(comp);
    }
    return pVec;
}

template<class T>
void GameObject::RemoveComponents()
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