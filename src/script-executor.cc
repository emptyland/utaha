#include "script-executor.h"
#include "lua-utils.h"
#include "lazy-instance.h"

namespace utaha {

LazyInstance<ScriptExecutor> SCRIPTS(LAZY_INSTANCE_INITIALIZER);

ScriptExecutor::ScriptExecutor() {
}

ScriptExecutor::~ScriptExecutor() {
}

lua_State *ScriptExecutor::ExecStandaloneFile(const char *file,
                                              char const **err) {
    auto L = LuaUtils::GeneralOpenLua();
    if (!L) {
        *err = "Can not open lua state.";
        return nullptr;
    }

    *err = LuaUtils::ProtectedDoFile(L, file);
    return L;
}

lua_State *
ScriptExecutor::ExecStandaloneFile(std::function<void(lua_State *)> init,
                                   const char *file, char const **err) {
    auto L = LuaUtils::GeneralOpenLua();
    if (!L) {
        *err = "Can not open lua state.";
        return nullptr;
    }
    init(L);
    *err = LuaUtils::ProtectedDoFile(L, file);
    return L;
}

ScriptScope::~ScriptScope() {
    while (!lua_vms_.empty()) {
        auto L = lua_vms_.front();
        lua_close(L);
        lua_vms_.pop_front();
    }
}

} // namespace utaha
