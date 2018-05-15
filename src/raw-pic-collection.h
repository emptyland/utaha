#ifndef UTAHA_RAW_PIC_COLLECTION_H_
#define UTAHA_RAW_PIC_COLLECTION_H_

#include "base.h"
#include <string>
#include <map>

typedef struct SDL_Surface SDL_Surface;
typedef struct lua_State lua_State;

namespace utaha {

class RawPicCollection {
public:
    RawPicCollection();
    ~RawPicCollection();

    void AddFile(const char *file);
    SDL_Surface *FindPicOrNull(const char *file);

    DEF_VAL_PROP_RMW(std::string, dir);

    size_t LoadWithBootstrapScript(const char *bootstrap_script_file,
                                   std::string *err);

    static void BindTo(lua_State *L);

    DISALLOW_IMPLICIT_CONSTRUCTORS(RawPicCollection);
private:
    void SetDirWarp(const char *dir);

    std::string dir_;
    std::map<std::string, SDL_Surface *> surfaces_;
}; // class RawPicCollection

} // namespace utaha

#endif // UTAHA_RAW_PIC_COLLECTION_H_
