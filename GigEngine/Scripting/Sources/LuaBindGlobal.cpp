#include "LuaBindGlobal.h"
#include "Log.h"

void GigScripting::LuaBinderGlobal::BindGlobals(sol::state& pLuaState)
{
    auto& luaState = pLuaState;
    luaState.create_named_table("Debug");
    luaState["Debug"]["Log"] = [](const std::string& pMessage) { GIG_LOG(pMessage); };
}