#pragma once
#include <sol.hpp>
namespace GigScripting
{
    class LuaBindComponent
    {
    public:

        static void BindComponent(sol::state& pLuaState);
    };
}