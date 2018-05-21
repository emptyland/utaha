#ifndef UTAHA_GRID_PIC_STORAGE_H_
#define UTAHA_GRID_PIC_STORAGE_H_

#include "base.h"
#include <string>
#include <vector>
#include <map>

typedef struct SDL_Surface SDL_Surface;

namespace utaha {

class GridPicStorage {
public:
    GridPicStorage();
    ~GridPicStorage();

    DEF_VAL_PROP_RW(int, grid_w);
    DEF_VAL_PROP_RW(int, grid_h);
    DEF_VAL_PROP_RW(int, pitch);
    DEF_VAL_PROP_RW(std::string, dir);
    DEF_VAL_PROP_RW(std::string, name);
    DEF_VAL_PROP_RMW(std::vector<SDL_Surface *>, grid_pics)

    bool LoadFromFile();
    bool StoreToFile();

    int PutGrid(const std::string &original_file, int original_idx,
                SDL_Surface *grid, bool *ok);

    int AddGrid(SDL_Surface *grid, bool *ok);

    SDL_Surface *FindOrNullGrid(const std::string &original_file,
                                int original_idx, int *index) const;

    bool Exists(const std::string &original_file, int original_idx) const {
        return FindOrNullGrid(original_file, original_idx, nullptr) != nullptr;
    }

    DISALLOW_IMPLICIT_CONSTRUCTORS(GridPicStorage);
private:
    int grid_w_ = 0;
    int grid_h_ = 0;
    int pitch_ = 64;
    std::string dir_;
    std::string name_;
    std::vector<SDL_Surface *> grid_pics_;
    std::map<std::string, int> original_;
};

} // namespace utaha

#endif // UTAHA_GRID_PIC_STORAGE_H_
