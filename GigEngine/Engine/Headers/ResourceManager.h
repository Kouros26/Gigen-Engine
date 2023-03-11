#pragma once
#include "IResource.h"
#include <unordered_map>
#include <iostream>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	template <typename T>
	static T* Get(std::string const& filePath);

	static void DeleteRessource(const std::string& str);

private:
	template <typename T>
	static T* Create(std::string const& filePath);

	inline static std::unordered_map <std::string, std::unique_ptr<IResource>> resources;
};

template<typename T>
inline T* ResourceManager::Get(std::string const& filePath)
{
	auto it = resources.find(filePath);
	if (it == resources.end())
	{
		return Create<T>(filePath.c_str());
	}

	return static_cast <T*>(it->second.get());
}

template <typename T>
T* ResourceManager::Create(std::string const& filePath)
{
	if (!std::is_base_of_v<IResource, T>)
	{
		std::cout << "T is not a child of IResources" << std::endl;
		return nullptr;
	}

	//here threadpool
	resources[filePath] = std::make_unique<T>(filePath);

	return static_cast <T*>(resources[filePath].get());
}