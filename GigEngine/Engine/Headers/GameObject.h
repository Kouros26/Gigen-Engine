#pragma once
#include <list>

#include "Transform.h"
#include <vector>

class Component;
class Script;
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
	virtual void UpdateRender() const;
	void UpdateComponents() const;
	void UpdateHierarchy();

	void Destroy();

	void SetModel(const std::string& filePath);
	void SetTexture(const std::string& filePath);
	void LateUpdate() const;

	std::string GetName();
	unsigned int GetId();

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);

	void AddComponent(Component* newComponent);

	//create New component of type and return the new Component
	template<class T>
	T* AddComponent();

	//return first component of type
	template<class T>
	T* GetComponent();

	[[nodiscard]] Component* GetComponentByID(int id) const;

	//return vector of all components of type
	template<class T>
	std::vector<T*>& GetComponents();

	//remove all components of type
	template<class T>
	void RemoveComponents();

	[[nodiscard]] unsigned int GetComponentCount() const;

	Transform& GetTransform();

private:

	std::string name;
	unsigned int id;

	Transform transform;

	GameObject* parent = nullptr;
	std::list<GameObject*> children{};

	std::vector<Component*> components;
	std::vector<Script*> scripts;

	Model* model = nullptr;

	//use so every gameObject has a different id
	static unsigned int gameObjectIndex;
};

template<class T>
T* GameObject::AddComponent()
{
	T* newComp = new T(this);
	components.push_back(newComp);

	if (const auto script = dynamic_cast<Script*>(newComp))
	{
		scripts.push_back(script);
		script->Awake();
	}

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