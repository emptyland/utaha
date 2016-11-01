#ifndef UTAHA_TEXTURE_METADATA_H_
#define UTAHA_TEXTURE_METADATA_H_

#include "base.h"
#include "datatype.h"
#include "lua_utils.h"
#include <map>

namespace utaha {

class ResourceCollection {
public:
    ResourceCollection() {}

    virtual ~ResourceCollection() {}

    DISALLOW_IMPLICIT_CONSTRUCTORS(ResourceCollection)
private:

}; // class ResourceCollection

class Texture2DCollection : public ResourceCollection {
public:
    Texture2DCollection();
    virtual ~Texture2DCollection();

    static int LoadPNGFile(const char *png_file_path);

    int LuaCFunction(lua_State *L);

    Texture2DBinder *GetTex2DByName(const char *name);

private:
    int MakeTex2DBinder(const std::string &name, int tex_id, lua_State *L);

    std::map<std::string, Texture2DBinder> tex2d_binders_;
}; // class TextureMetadataCollection

} // namespace utaha

#endif // UTAHA_TEXTURE_METADATA_H_
