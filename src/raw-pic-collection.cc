#include "raw-pic-collection.h"
#include "glog/logging.h"
#include "lua-utils.h"
#include "script-executor.h"
#include SDL_H
#include SDL_IMAGE_H

namespace utaha {

RawPicCollection::RawPicCollection() {
}

RawPicCollection::~RawPicCollection() {
    for (const auto &pair : surfaces_) {
        if (pair.second) {
            SDL_FreeSurface(pair.second);
        }
    }
}

void RawPicCollection::AddFile(const char *file) {
    auto iter = surfaces_.find(file);
    if (iter != surfaces_.end()) {
        return;
    }
    surfaces_.emplace(file, nullptr);
}

SDL_Surface *RawPicCollection::FindPicOrNull(const char *file) {
    auto iter = surfaces_.find(file);
    if (iter == surfaces_.end()) {
        return nullptr;
    }
    if (iter->second) {
        return iter->second;
    }

    std::string file_name(dir_);
    if (!file_name.empty()) {
        file_name.append("/");
        file_name.append(file);
    }

    auto surface = IMG_Load(file_name.c_str());
    if (!surface) {
        LOG(ERROR) << "Can not load file: " << file << " " << SDL_GetError();
    } else {
        surfaces_.emplace(file, surface);
    }
    return surface;
}

size_t
RawPicCollection::LoadWithBootstrapScript(const char *bootstrap_script_file,
                                          std::string *err) {
    ScriptScope scripts(SCRIPTS.ptr());

    auto stub = this;
    const char *e = nullptr;
    scripts.ExecStandaloneFile([this, &stub] (lua_State *L) {
        BindTo(L);
        luabridge::getGlobalNamespace(L)
        .beginNamespace(kLuaNamespace)
            .addVariable("rawPicCollection", &stub, false)
        .endNamespace();
    }, bootstrap_script_file, &e);
    if (e) {
        err->assign(e);
    }
    return surfaces_.size();
}

/*static*/ void RawPicCollection::BindTo(lua_State *L) {
    luabridge::getGlobalNamespace(L)
    .beginNamespace(kLuaNamespace)
        .beginClass<RawPicCollection>("RowPicCollection")
            .addFunction("addFile", &RawPicCollection::AddFile)
            .addFunction("setDir", &RawPicCollection::SetDirWarp)
        .endClass()
    .endNamespace();
}

void RawPicCollection::SetDirWarp(const char *dir) {
    set_dir(dir);
}

} // namespace utaha
