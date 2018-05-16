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
        if (std::get<1>(pair.second)) {
            SDL_FreeSurface(std::get<1>(pair.second));
        }
    }
}

void RawPicCollection::AddFile(const char *file) {
    AddFileWithMask(file, 0xff);
}

void RawPicCollection::AddFileWithMask(const char *file, uint32_t alpha_mask) {
    auto iter = surfaces_.find(file);
    if (iter != surfaces_.end()) {
        return;
    }
    surfaces_.emplace(file, std::make_tuple(alpha_mask, nullptr));
}

SDL_Surface *RawPicCollection::FindPicOrNull(const char *file) {
    auto iter = surfaces_.find(file);
    if (iter == surfaces_.end()) {
        return nullptr;
    }

    if (std::get<1>(iter->second)) {
        return std::get<1>(iter->second);
    }
    std::string file_name(dir_);
    if (!file_name.empty()) {
        file_name.append("/");
        file_name.append(file);
    }

    auto surface = IMG_Load(file_name.c_str());
    if (!surface) {
        LOG(ERROR) << "Can not load file: " << file_name << " reason: " << IMG_GetError();
        return nullptr;
    }
    const auto alpha_mask = std::get<0>(iter->second);
    if (alpha_mask != 0xff) {
        if ((surface = ProcessAlphaMask(surface, alpha_mask)) == nullptr) {
            return nullptr;
        }
    }
    surfaces_[file] = std::make_tuple(alpha_mask, surface);
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
            .addFunction("addFileWithMask", &RawPicCollection::AddFileWithMask)
            .addFunction("setDir", &RawPicCollection::SetDirWarp)
        .endClass()
    .endNamespace();
}

void RawPicCollection::SetDirWarp(const char *dir) {
    set_dir(dir);
}

SDL_Surface *RawPicCollection::ProcessAlphaMask(SDL_Surface *surface, uint32_t alpha_mask) {
    auto regular = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    if (!regular) {
        LOG(ERROR) << "Can not convert surface. " << SDL_GetError();
        SDL_FreeSurface(surface);
        return nullptr;
    }
    SDL_FreeSurface(surface);

    if (SDL_LockSurface(regular) != 0) {
        LOG(ERROR) << "Can not lock surface. " << SDL_GetError();
        SDL_FreeSurface(regular);
        return nullptr;
    }
    int processed_pixels = 0;
    Uint32 *pixels = static_cast<Uint32 *>(regular->pixels);
    for (int i = 0; i < regular->w * regular->h; ++i) {
        SDL_Color pixel;
        SDL_GetRGBA(pixels[i], regular->format, &pixel.r, &pixel.g, &pixel.b,
                    &pixel.a);

        Uint32 color = static_cast<Uint32>(pixel.r) << 24
            | static_cast<Uint32>(pixel.g) << 16
            | static_cast<Uint32>(pixel.b) << 8
            | 0x00;
        if (color == alpha_mask) {
            pixels[i] = SDL_MapRGBA(regular->format, 0, 0, 0, 0);
            ++processed_pixels;
        }
    }
    //DLOG(INFO) << "processed pixels: " << processed_pixels;

    SDL_UnlockSurface(regular);
    return regular;
}

} // namespace utaha
