#ifndef UTAHA_SCRIPT_EXECUTOR_H_
#define UTAHA_SCRIPT_EXECUTOR_H_

#include "base.h"
#include "lazy-instance.h"
#include "glog/logging.h"
#include "lua.hpp"
#include <functional>
#include <list>

typedef struct lua_State lua_State;

namespace utaha {

class ScriptExecutor {
public:
    ScriptExecutor();
    ~ScriptExecutor();

    lua_State *ExecStandaloneFile(const char *file, char const **err);

    lua_State *ExecStandaloneFile(std::function<void(lua_State *)> init,
                                  const char *file, char const **err);

    DISALLOW_IMPLICIT_CONSTRUCTORS(ScriptExecutor);
}; // class ScriptExecutor

class ScriptScope {
public:
    ScriptScope(ScriptExecutor *executor)
        : executor_(DCHECK_NOTNULL(executor)) {}
    ~ScriptScope();

    inline lua_State *ExecStandaloneFile(const char *file, char const **err);

    inline lua_State *ExecStandaloneFile(std::function<void(lua_State *)> init,
                                         const char *file, char const **err);

    DISALLOW_IMPLICIT_CONSTRUCTORS(ScriptScope);
private:
    ScriptExecutor *executor_;
    std::list<lua_State *> lua_vms_;
}; // class ScriptScope

inline lua_State *ScriptScope::ExecStandaloneFile(const char *file,
                                                  char const **err) {
    auto L = executor_->ExecStandaloneFile(file, err);
    if (*err) {
        lua_close(L);
        return nullptr;
    }
    lua_vms_.push_front(L);
    return L;
}

inline lua_State *
ScriptScope::ExecStandaloneFile(std::function<void(lua_State *)> init,
                                const char *file, char const **err) {
    auto L = executor_->ExecStandaloneFile(init, file, err);
    if (*err) {
        lua_close(L);
        return nullptr;
    }
    lua_vms_.push_front(L);
    return L;
}

extern LazyInstance<ScriptExecutor> SCRIPTS;

} // namespace utaha

#endif // UTAHA_SCRIPT_EXECUTOR_H_
