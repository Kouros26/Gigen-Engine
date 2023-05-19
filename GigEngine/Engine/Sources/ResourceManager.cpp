#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{

}

inline void ResourceManager::DeleteResource(const std::string& filePath)
{
    auto it = resources.find(filePath);
    if (it != resources.end())
    {
        resources.erase(it);
    }
}

void ResourceManager::Cleanup()
{
    if (!resources.empty())
    {
        resources.clear();
    }
}

inline ThreadPool ResourceManager::threadPool(std::thread::hardware_concurrency() - 1);