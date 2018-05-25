#ifndef UTAHA_UNIVERSAL_PROFILE_H_
#define UTAHA_UNIVERSAL_PROFILE_H_

#include "base.h"
#include <string>

typedef struct lua_State lua_State;

namespace utaha {

class UniversalProfile {
public:
    UniversalProfile();
    ~UniversalProfile();

    DEF_VAL_PROP_RW(int, tile_w);
    DEF_VAL_PROP_RW(int, tile_h);
    DEF_VAL_PROP_RW(int, avatar_w);
    DEF_VAL_PROP_RW(int, avatar_h);
    DEF_VAL_PROP_RW(std::string, assets_dir);
    DEF_VAL_PROP_RW(std::string, raw_pic_load_file);
    DEF_VAL_PROP_RW(std::string, editor_styles_file);
    DEF_VAL_PROP_RW(std::string, editor_layout_file);

    void LoadFromFile(const char *file, std::string *err);

    static void BindTo(lua_State *L);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UniversalProfile);
private:
    int ApplyProfiles(lua_State *L);

    int tile_w_ = 48;
    int tile_h_ = 48;

    int avatar_w_ = 24;
    int avatar_h_ = 32;
    std::string assets_dir_ = "assets";

    ////////////////////////////////////////////////////////////////////////////
    // Editor Profiles:
    std::string raw_pic_load_file_ = "res/raw-pic-load.lua";
    std::string editor_styles_file_ = "res/styles.lua";
    std::string editor_layout_file_ = "res/form-layouts.lua";
    ////////////////////////////////////////////////////////////////////////////
}; // class UniversalProfile

} // namespace utaha

#endif // UTAHA_UNIVERSAL_PROFILE_H_
