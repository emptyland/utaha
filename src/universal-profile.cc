#include "universal-profile.h"
#include "script-executor.h"
#include "lua-utils.h"

namespace utaha {

UniversalProfile::UniversalProfile() {
}

UniversalProfile::~UniversalProfile() {
}

void UniversalProfile::LoadFromFile(const char *file, std::string *err) {
    ScriptScope scripts(SCRIPTS.ptr());

    auto stub = this;
    const char *e = nullptr;
    scripts.ExecStandaloneFile([&stub](lua_State *L) {
        BindTo(L);
        luabridge::getGlobalNamespace(L)
        .beginNamespace(kLuaNamespace)
            .addVariable("profile", &stub)
        .endNamespace();
    }, file, &e);
    if (e) {
        err->assign(e);
    }
}

/*static*/ void UniversalProfile::BindTo(lua_State *L) {
    luabridge::getGlobalNamespace(L)
    .beginNamespace(kLuaNamespace)
        .beginClass<UniversalProfile>("UniversalProfile")
            .addCFunction("apply", &UniversalProfile::ApplyProfiles)
        .endClass()
    .endNamespace();
    //.endNamespace();
}



int UniversalProfile::ApplyProfiles(lua_State *L) {
    LuaUtils::EnsureArguments(L, 1);

    luabridge::LuaRef arg = luabridge::LuaRef::fromStack(L, -1);
    if (!arg.isTable()) {
        luaL_error(L, "Argument is not table.");
    }

    if (!arg["tileW"].isNil()) {
        tile_w_ = arg["tileW"].cast<int>();
    }
    if (!arg["tileH"].isNil()) {
        tile_h_ = arg["tileH"].cast<int>();
    }
    if (!arg["avatarW"].isNil()) {
        avatar_w_ = arg["avatarW"].cast<int>();
    }
    if (!arg["avatarH"].isNil()) {
        avatar_h_ = arg["avatarH"].cast<int>();
    }
    if (!arg["assetsDir"].isNil()) {
        assets_dir_ = arg["assetsDir"].cast<std::string>();
    }

    luabridge::LuaRef editor = arg["editor"];
    if (editor.isTable()) {
        if (!editor["rawPicLoadFile"].isNil()) {
            raw_pic_load_file_ = editor["rawPicLoadFile"].cast<std::string>();
        }
        if (!editor["stylesFile"].isNil()) {
            editor_styles_file_ = editor["stylesFile"].cast<std::string>();
        }
        if (!editor["layoutFile"].isNil()) {
            editor_layout_file_ = editor["layoutFile"].cast<std::string>();
        }
        if (!editor["windowW"].isNil()) {
            editor_window_w_ = editor["windowW"].cast<int>();
        }
        if (!editor["windowH"].isNil()) {
            editor_window_h_ = editor["windowH"].cast<int>();
        }
    }
    return 0;
}

} // namespace utaha
