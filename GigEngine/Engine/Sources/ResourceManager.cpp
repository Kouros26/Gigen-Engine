#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	resources.clear();
}

void ResourceManager::DeleteRessource(const std::string& pStr)
{
	auto it = resources.find(pStr);
	if (it != resources.end())
	{
		resources.erase(it);
	}
}