#include "ui-style-collection.h"
#include "glog/logging.h"
#include "lua-utils.h"
#include <SDL2_ttf/SDL_ttf.h>

namespace utaha {

UIStyleCollection::UIStyleCollection() {
}

UIStyleCollection::~UIStyleCollection(){
    for (const auto &pair : fonts_) {
        TTF_CloseFont(pair.second);
    }
}

bool UIStyleCollection::AddFont(const std::string &name,
                                const std::string &ttf_file_path,
                                int size) {
    TTF_Font *font = TTF_OpenFont(ttf_file_path.c_str(), size);
    if (!font) {
        LOG(ERROR) << "can not open font: " << ttf_file_path;
        return false;
    }
    auto iter = fonts_.find(name);
    if (iter != fonts_.end()) {
        TTF_CloseFont(iter->second);
    }
    fonts_.emplace(name, font);
    return true;
}

const char *UIStyleCollection::LoadFromFile(const char *file_name) {
    lua_State *L = LuaUtils::GeneralOpenLua();
    if (!L) {
        return "Can not open lua!";
    }
    BindTo(L);

    auto pointer_stub = this;
    luabridge::getGlobalNamespace(L)
        .beginNamespace(kLuaNamespace)
            .addVariable("styleCollection", &pointer_stub, false)
        .endNamespace();
    const char *err = LuaUtils::ProtectedDoFile(L, file_name);
    lua_close(L);
    L = nullptr;
    return err;
}

/*static*/ int UIStyleCollection::BindTo(lua_State *L) {
    luabridge::getGlobalNamespace(L)
        .beginNamespace(kLuaNamespace)
            .beginClass<UIStyleCollection>("StyleCollection")
                .addConstructor<void(*)(void)>()
                .addCFunction("addFont", &UIStyleCollection::WarpAddFont)
                .addCFunction("setFont", &UIStyleCollection::WarpSetFont)
                .addCFunction("setColor", &UIStyleCollection::WarpSetColor)
                .addFunction("setSize", &UIStyleCollection::WarpSetSize)
                .addFunction("setInt", &UIStyleCollection::WarpSetInt)
                .addFunction("setFloat", &UIStyleCollection::WarpSetFloat)
            .endClass()
        .endNamespace();
    return 0;
}

int UIStyleCollection::WarpAddFont(lua_State *L) {
    int argc = lua_gettop(L);
    if (argc != 3 + 1) { // this pinter is first argument.
        luaL_error(L, "incorrect arguments number %d, need %d", argc, 3);
    }

    const char *name = luaL_checkstring(L, 2);
    const char *path = luaL_checkstring(L, 3);
    int size = luaL_checkint(L, 4);

    if (!AddFont(name, path, size)) {
        luaL_error(L, "can not open ttf-font file: %s", path);
    }

    luabridge::push(L, this);
    return 1;
}

int UIStyleCollection::WarpSetFont(lua_State *L) {
    int argc = lua_gettop(L);
    if (argc != 2 + 1) { // this pinter is first argument.
        luaL_error(L, "incorrect arguments number %d, need %d", argc, 2);
    }

    const char *name = luaL_checkstring(L, 2);
    const char *font_name = luaL_checkstring(L, 3);

    if (!SetFont(name, font_name)) {
        luaL_error(L, "can not font named: %s", font_name);
    }

    luabridge::push(L, this);
    return 1;
}

int UIStyleCollection::WarpSetColor(lua_State *L) {
    int argc = lua_gettop(L);
    if (argc != 2 + 1) { // this pinter is first argument.
        luaL_error(L, "incorrect arguments number %d, need %d", argc, 2);
    }

    const char *name = luaL_checkstring(L, 2);
    uint32_t color = luaL_checkint(L, 3);

    SetColor(name, (color & 0xff000000) >> 24, (color & 0x00ff0000) >> 16,
             (color & 0x0000ff00) >> 8, color & 0x000000ff);

    luabridge::push(L, this);
    return 1;
}

UIStyleCollection *UIStyleCollection::WarpSetSize(const char *name, int w, int h) {
    SetSize(name, w, h);
    return this;
}

UIStyleCollection *UIStyleCollection::WarpSetInt(const char *name, int val) {
    SetInt(name, val);
    return this;
}

UIStyleCollection *UIStyleCollection::WarpSetFloat(const char *name, float val) {
    SetFloat(name, val);
    return this;
}

} // namespace utaha
