#pragma once
#include "Transform.h"
#include <vector>

class GameObject
{
public:
	GameObject();
	GameObject(std::string name);
	virtual ~GameObject();

	void Destroy();

	void UpdateRender() const;
	void UpdateComponents() const;

	std::string GetName();
	unsigned int GetId();

	void setModel(std::string const& filePath);

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

	Transform transform;
private:
	std::string name;
	unsigned int id;

	std::vector<class Component*> components;
	class Model* model = nullptr;

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
		if (T* comp = dynamic_cast<T*>(components[i]))
		{
			delete comp;
			components.erase(components.begin() + i);
		}
	}
}