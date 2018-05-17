#ifndef UTAHA_RAW_PIC_COLLECTION_H_
#define UTAHA_RAW_PIC_COLLECTION_H_

#include "base.h"
#include <string>
#include <map>
#include <tuple>
#include <vector>

typedef struct SDL_Surface SDL_Surface;
typedef struct lua_State lua_State;

namespace utaha {

class RawPicCollection {
public:
    RawPicCollection();
    ~RawPicCollection();

    void AddFile(const char *file);
    void AddFileWithMask(const char *file, uint32_t alpha_mask);
    SDL_Surface *FindPicOrNull(const char *file);

    size_t GetAllFileNames(std::vector<std::string> *names);

    DEF_VAL_PROP_RMW(std::string, dir);

    size_t LoadWithBootstrapScript(const char *bootstrap_script_file,
                                   std::string *err);

    static void BindTo(lua_State *L);

    DISALLOW_IMPLICIT_CONSTRUCTORS(RawPicCollection);
private:
    void SetDirWarp(const char *dir);
    SDL_Surface *ProcessAlphaMask(SDL_Surface *surface, uint32_t alpha_mask);

    std::string dir_;
    std::map<std::string, std::tuple<uint32_t, SDL_Surface *>> surfaces_;
}; // class RawPicCollection

} // namespace utaha

#endif // UTAHA_RAW_PIC_COLLECTION_H_
