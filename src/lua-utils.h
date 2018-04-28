#ifndef UTAHA_BASE_LUA_UTILS_H_
#define UTAHA_BASE_LUA_UTILS_H_

#include "base.h"
#include "glog/logging.h"
#include "lua.hpp"
#include "LuaBridge.h"
#include <stdio.h>
#include <type_traits>
#include <tuple>

namespace utaha {

namespace detail {

template<class T, bool IsEnum>
struct EnumWarpper {
    static T Get(lua_State *L, int idx) {
        return luabridge::Stack<T>::get(L, -1);
    }
};

template<class T>
struct EnumWarpper<T, true> {
    static T Get(lua_State *L, int idx) {
        return static_cast<T>(luabridge::Stack<int>::get(L, -1));
    }
};

} // namespace detail

namespace d = detail;

struct LuaUtils {

    template<class T>
    static
    inline std::tuple<T, bool> GetByFieldName(lua_State *L, const char *name) {
        std::tuple<T, bool> result;
        luaL_checktype(L, -1, LUA_TTABLE);

        lua_pushstring(L, name);
        lua_rawget(L, -2);
        if (lua_isnil(L, -1)) {
            std::get<1>(result) = false;
        } else {
            std::get<0>(result) = luabridge::Stack<T>::get(L, -1);
            std::get<1>(result) = true;
        }
        lua_pop(L, 1);
        return result;
    }

    template<class T>
    static
    inline std::tuple<T, bool> GetByIndex(lua_State *L, int index) {
        std::tuple<T, bool> result;
        luaL_checktype(L, -1, LUA_TTABLE);

        lua_pushinteger(L, index);
        lua_rawget(L, -2);
        if (lua_isnil(L, -1)) {
            std::get<1>(result) = false;
        } else {
            std::get<0>(result) = luabridge::Stack<T>::get(L, -1);
            std::get<1>(result) = true;
        }
        lua_pop(L, 1);
        return result;
    }

    template<class T>
    static
    inline T GetByFieldName(lua_State *L, const char *name,
                            const T &default_value) {
        T result;
        luaL_checktype(L, -1, LUA_TTABLE);

        lua_pushstring(L, name);
        lua_rawget(L, -2);
        if (lua_isnil(L, -1)) {
            result = default_value;
        } else {
            result = luabridge::Stack<T>::get(L, -1);
        }
        lua_pop(L, 1);
        return result;
    }

    template<class T>
    static
    inline T RequireByFieldName(lua_State *L, const char *name) {
        T result;
        luaL_checktype(L, -1, LUA_TTABLE);

        lua_pushstring(L, name);
        lua_rawget(L, -2);
        if (lua_isnil(L, -1)) {
            lua_pop(L, 1);
            char buf[64];
            snprintf(buf, sizeof(buf), "miss field: \"%s\"", name);
            luaL_argerror(L, -1, buf);
            return T(0);
        }

        result = d::EnumWarpper<T, std::is_enum<T>::value>::Get(L, -1);
        lua_pop(L, 1);
        return result;
    }

    static
    inline int RequireTableByFieldName(lua_State *L, const char *name) {
        luaL_checktype(L, -1, LUA_TTABLE);

        lua_pushstring(L, name);
        lua_rawget(L, -2);
        if (lua_isnil(L, -1)) {
            lua_pop(L, 1);
            char buf[64];
            snprintf(buf, sizeof(buf), "miss field: \"%s\"", name);
            return luaL_argerror(L, -1, buf);
        }

        DCHECK(lua_istable(L, -2));
        return 0;
    }

    static
    inline lua_State *GeneralOpenLua() {
        auto L = luaL_newstate();
        if (!L) {
            return nullptr;
        }
        luaopen_jit(L);
        luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);
        luaL_openlibs(L);
        return L;
    }

    static
    inline const char *ProtectedDoFile(lua_State *L, const char *file_name) {
        int result;
        result = luaL_loadfile(L, file_name);
        if (result != 0) {
            return lua_tostring(L, -1);
        }
        result = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (result != 0) {
            return lua_tostring(L, -1);
        }
        return nullptr;
    }

    static 
    inline const char *ProtectedDoString(lua_State *L, const char *string) {
        int result;
        result = luaL_loadstring(L, string);
        if (result != 0) {
            return lua_tostring(L, -1);
        }
        result = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (result != 0) {
            return lua_tostring(L, -1);
        }
        return nullptr;
    }

    static
    inline int EnsureArguments(lua_State *L, int argc) {
        auto origin_top = lua_gettop(L);
        if (origin_top < argc) {
            return luaL_error(L, "bad argument number. expected: %d",
                              origin_top);
        }
        return origin_top;
    }
}; // class LuaUtils


/**
 * LuaIterator iter(L);
 * for (iter.Init(); iter.Valid(); iter.Next()) {
 *     auto key = iter.key<int>();
 *     auto val = iter.value<std::string>();
 *     // Use key/val...
 * }
 * iter.Done();
 *
 */
class LuaIterator {
public:
    LuaIterator(lua_State *state, int index)
        : L(state)
        , index_(index) {
        DCHECK(lua_istable(L, index_));
    }

    void Init() {
        lua_pushnil(L);
        ok_ = lua_next(L, index_ - 1);
    }

    bool Valid() {
        return ok_;
    }

    void Next() {
        DCHECK(Valid());
        lua_pop(L, 1); // pop value, keeping key in stack
        ok_ = lua_next(L, index_ - 1);
    }

    template<class T>
    inline T key() {
        DCHECK(Valid());
        return luabridge::Stack<T>::get(L, -2);
    }

    template<class T>
    inline T value() {
        DCHECK(Valid());
        return luabridge::Stack<T>::get(L, -1);
    }

private:
    lua_State *L = nullptr;
    int index_ = -1;
    bool ok_ = false;

}; // class LuaIterator

extern const char kLuaNamespace[];
extern const char kLuaConstTableName[];

} // namespace utaha

#endif // UTAHA_BASE_LUA_UTILS_H_
