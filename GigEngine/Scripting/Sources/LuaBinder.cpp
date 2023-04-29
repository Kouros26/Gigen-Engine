#include "LuaBinder.h"
#include "LuaBindGlobal.h"
#include "LuaBindMath.h"
void GigScripting::LuaBinder::Bind(sol::state& pLuaState)
{
    //TODO: bind lua functions from other binder
    auto& luaState = pLuaState;
    GigScripting::LuaBinderGlobal::BindGlobals(luaState);
    GigScripting::LuaBindMath::BindMath(luaState);
}