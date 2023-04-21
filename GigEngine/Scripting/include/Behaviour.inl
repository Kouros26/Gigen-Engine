#pragma once
#include <string>

namespace GigScripting
{
    template<typename ...Args>
    void Behaviour::CallLuaFunction(const std::string& functionName, Args&&... args)
    {
        scriptTable[functionName](std::forward<Args>(args)...);
    }
}