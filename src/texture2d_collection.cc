#include "texture2d_collection.h"
#include "lodepng.h"
#include <OpenGL/gl.h>

namespace utaha {

Texture2DCollection::Texture2DCollection() {
}

Texture2DCollection::~Texture2DCollection() {
}

/*static*/
int Texture2DCollection::LoadPNGFile(const char *png_file_path) {
    std::vector<unsigned char> image;
    unsigned int width, height;
    auto err = lodepng::decode(image, width, height, png_file_path);
    if (err) {
        LOG(ERROR) << "can not load file: " << png_file_path;
        return -1;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, &image[0]);
    return tex_id;
}

/**
 * {
 *    file = 'assets/test.png'
 *    tex2d = {
 *        'role.main.1' = {
 *            {1, 1}, {1, 2}, {2, 3}, {3, 4}
 *        }
 *    }
 * }
 *
 */
int Texture2DCollection::LuaCFunction(lua_State *L) {
    luaL_checktype(L, -1, LUA_TTABLE);

    auto png_file_path = LuaUtils::RequireByFieldName<std::string>(L, "file");
    auto tex_id = LoadPNGFile(png_file_path.c_str());
    if (tex_id < 0) {
        return luaL_error(L, "texture file: %s can not open!",
                          png_file_path.c_str());
    }

    LuaUtils::RequireTableByFieldName(L, "tex2d");
    LuaIterator iter(L, -1);
    for (iter.Init(); iter.Valid(); iter.Next()) {
        MakeTex2DBinder(iter.key<std::string>(), tex_id, L);
    }
    lua_pop(L, 1);
    return 0;
}

Texture2DBinder *Texture2DCollection::GetTex2DByName(const char *name) {
    auto iter = tex2d_binders_.find(name);
    return iter == tex2d_binders_.end() ? &iter->second : nullptr;
}

int Texture2DCollection::MakeTex2DBinder(const std::string &name, int tex_id,
                                         lua_State *L) {
    Texture2DBinder tex2d;

    tex2d.tex_id = tex_id;
    auto vertex = &tex2d._1;

    for (int i = 0; i < 4; i++) {
        lua_pushinteger(L, i);
        lua_rawget(L, -2);
        if (!lua_istable(L, -1)) {
            return -1;
        }

        lua_pushinteger(L, 0);
        lua_rawget(L, -2);
        if (!lua_isnumber(L, -1)) {
            return -1;
        }
        vertex->x = lua_tointeger(L, -1);
        lua_pop(L, 1);

        
        lua_pop(L, 1);
    }

    return 0;
}

} // namespace utaha
