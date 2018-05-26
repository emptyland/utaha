#ifndef UTAHA_FIXED_TERRAIN_H_
#define UTAHA_FIXED_TERRAIN_H_

#include "base.h"

namespace utaha {

class FixedTerrain {
public:
    FixedTerrain();
    ~FixedTerrain();

    DISALLOW_IMPLICIT_CONSTRUCTORS(FixedTerrain);
private:
    int id_;
    int tile_w_ = 0;
    int tile_h_ = 0;
    int max_h_tiles_ = 0;
    int max_v_tiles_ = 0;
    int *tiles_ = nullptr;
}; // class FixedTerrain

} // namespace utaha

#endif // UTAHA_FIXED_TERRAIN_H_
