template<typename T>
inline T* ResourceManager::Get(const std::string& filePath)
{
    auto it = resources.end();
    if (resources.size() > 0)
    {
        it = resources.find(filePath);
    }
    if (it == resources.end())
    {
        return Create<T>(filePath);
    }

    return static_cast<T*>(it->second.get());
}

template <typename T>
inline T* ResourceManager::Create(const std::string& filePath)
{
    if (!std::is_base_of_v<IResource, T>)
    {
        std::cout << "T is not a child of IResources" << std::endl;
        return nullptr;
    }

    std::promise<std::unique_ptr<T>> resourcePromise;
    std::future<std::unique_ptr<T>> futureResource = resourcePromise.get_future();

    threadPool.Enqueue([filePath, &resourcePromise]() {
        std::unique_ptr<T> resource = std::make_unique<T>(filePath);
        resourcePromise.set_value(std::move(resource));
        });

    futureResource.wait();

    std::unique_ptr<T> resource = futureResource.get();

    resources[filePath] = std::move(resource);
    resources[filePath]->Init();
    return static_cast<T*>(resources[filePath].get());
}