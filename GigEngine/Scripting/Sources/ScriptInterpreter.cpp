#include "ScriptInterpreter.h"
#include "lua.h"

GigScripting::ScriptInterpreter::~ScriptInterpreter()
{
    DestroyLuaContext();
}

void GigScripting::ScriptInterpreter::CreateLuaContextAndBindGlobal()
{
    if (!luaState)
    {
        luaState = std::make_unique<sol::state>();
        luaState->open_libraries(sol::lib::base, sol::lib::math);
        //TODO: call binder
        isOk = true;

        for (const auto& behaviour : behaviours)
        {
            if (!behaviour->RegisterToLuaContext(scriptFolderRoot, *luaState))
            {
                isOk = false;
            }
        }
        if (!isOk)
        {
            std::cout << "Script interpreter failed to register scripts.Check your lua scripts" << std::endl;
        }
    }
}

void GigScripting::ScriptInterpreter::DestroyLuaContext()
{
    if (luaState)
    {
        for (const auto& behaviour : behaviours)
        {
            behaviour->UnregisterFromLuaContext();
        }
        luaState.reset();
        isOk = false;
    }
}

void GigScripting::ScriptInterpreter::RegisterBehaviour(Behaviour* pBehaviour)
{
    if (!luaState)
    {
        return;
    }

    behaviours.push_back(pBehaviour);
    if (!pBehaviour->RegisterToLuaContext(scriptFolderRoot, *luaState))
    {
        isOk = false;
    }
}

void GigScripting::ScriptInterpreter::UnregisterBehaviour(Behaviour* pBehaviour)
{
    if (!luaState)
    {
        return;
    }

    pBehaviour->UnregisterFromLuaContext();
    behaviours.erase(std::remove_if(behaviours.begin(), behaviours.end(), [pBehaviour](const auto& behaviour) { return pBehaviour == behaviour; }), behaviours.end());
    RefreshBehaviours();
}

void GigScripting::ScriptInterpreter::RefreshBehaviours()
{
    DestroyLuaContext();
    CreateLuaContextAndBindGlobal();
}

bool GigScripting::ScriptInterpreter::IsOk() const
{
    return isOk;
}

GigScripting::ScriptInterpreter::ScriptInterpreter(const std::string& pScriptFolderRoot)
{
    scriptFolderRoot = pScriptFolderRoot;
    CreateLuaContextAndBindGlobal();
}