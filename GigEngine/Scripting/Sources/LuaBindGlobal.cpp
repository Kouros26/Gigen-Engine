#include "LuaBindGlobal.h"
#include "Log.h"
#include "Inputs.h"
#include "Vec2/FVec2.hpp"

void GigScripting::LuaBinderGlobal::BindGlobals(sol::state& pLuaState)
{
	using namespace GigInput;

	auto& luaState = pLuaState;

	luaState.new_enum<Keys>("Keys",
		{
			{"Escape", Keys::ESCAPE},
			{"Space", Keys::SPACE},
			{"W", Keys::W},
			{"A", Keys::A},
			{"S", Keys::S},
			{"D", Keys::D},
			{"Up", Keys::UP},
			{"Down", Keys::DOWN},
			{"Left", Keys::LEFT},
			{"Right", Keys::RIGHT},
			{"LeftShift", Keys::LEFT_SHIFT},
			{"RightShift", Keys::RIGHT_SHIFT},
			{"LeftControl", Keys::LEFT_CONTROL},
			{"RightControl", Keys::RIGHT_CONTROL},
			{"LeftAlt", Keys::LEFT_ALT},
			{"RightAlt", Keys::RIGHT_ALT},
			{"Tab", Keys::TAB},
			{"Enter", Keys::ENTER},
			{"Backspace", Keys::BACKSPACE},
			{"F1", Keys::F1},
			{"F2", Keys::F2},
			{"F3", Keys::F3},
			{"F4", Keys::F4},
			{"F5", Keys::F5},
			{"F6", Keys::F6},
			{"F7", Keys::F7},
			{"F8", Keys::F8},
			{"F9", Keys::F9},
			{"F10", Keys::F10},
			{"F11", Keys::F11},
			{"F12", Keys::F12},
			{"Last", Keys::LAST}
		});

	luaState.new_enum<MouseButton>("MouseButtons",
		{
			{"Left", MouseButton::LEFT},
			{"Right", MouseButton::RIGHT},
			{"Middle", MouseButton::MIDDLE}
		});

	luaState.create_named_table("Debug");
	luaState.create_named_table("Inputs");

	luaState["Debug"]["Log"] = [](const std::string& pMessage) { GIG_LOG(pMessage); };
	luaState["Debug"]["LogWarning"] = [](const std::string& pMessage) { GIG_WARNING(pMessage); };
	luaState["Debug"]["LogError"] = [](const std::string& pMessage) { GIG_ERROR(pMessage); };

	luaState["Inputs"]["GetKey"] = [](Keys pKey) { return Inputs::GetKey(pKey); };
	luaState["Inputs"]["GetKeyDown"] = [](Keys pKey) { return Inputs::GetKeyDown(pKey); };
	luaState["Inputs"]["GetKeyUp"] = [](Keys pKey) { return Inputs::GetKeyUp(pKey); };
	luaState["Inputs"]["GetMouse"] = []() { return Inputs::GetMouse(); };
	luaState["Inputs"]["GetMousePos"] = []()
	{
		auto mouse = Inputs::GetMouse();
		return lm::FVec2(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
	};
}