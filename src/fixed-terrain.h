#ifndef UTAHA_FIXED_TERRAIN_H_
#define UTAHA_FIXED_TERRAIN_H_

#include "glog/logging.h"
#include "base.h"
#include <string>

namespace utaha {

struct IndexedLinker {
    int x;
    int y;
    int params;
    int to_id;
    int to_x;
    int to_y;
}; // struct IndexedTrap

struct IndexedEntity {
    int x;
    int y;
    int spirit_id;
}; // struct IndexedEntity

struct IndexedActor {
    int x;
    int y;
    int lv;
    int param;
    int actor_id;
}; // struct IndexedActor

class FixedTerrain {
public:
    FixedTerrain();
    ~FixedTerrain();

    DEF_VAL_PROP_RW(int, id);
    DEF_VAL_PROP_RW(std::string, name);
    DEF_VAL_PROP_RW(int, tile_w);
    DEF_VAL_PROP_RW(int, tile_h);
    DEF_VAL_PROP_RW(int, script_id);
    DEF_VAL_PROP_RW(int, max_h_tiles);
    DEF_VAL_PROP_RW(int, max_v_tiles);
    DEF_PTR_PROP_RW(int, tiles);
    DEF_PTR_PROP_RW(IndexedLinker, linkers);
    DEF_VAL_PROP_RW(int, linker_size);
    DEF_PTR_PROP_RW(IndexedEntity, entities);
    DEF_VAL_PROP_RW(int, entity_size);
    DEF_PTR_PROP_RW(IndexedActor, actors);
    DEF_VAL_PROP_RW(int, actor_size);

    int tile_size() const { return max_h_tiles_ * max_v_tiles_; }
    inline int tile(int x_tile, int y_tile) const;

    const IndexedLinker &linker(int i) const {
        DCHECK_GE(i, 0);
        DCHECK_LT(i, linker_size_);
        return linkers_[i];
    }

    const IndexedEntity &entity(int i) const {
        DCHECK_GE(i, 0);
        DCHECK_LT(i, entity_size_);
        return entities_[i];
    }

    const IndexedActor &actor(int i) const {
        DCHECK_GE(i, 0);
        DCHECK_LT(i, actor_size_);
        return actors_[i];
    }

    int pixels_w() const { return tile_w_ * max_h_tiles_; }
    int pixels_h() const { return tile_h_ * max_v_tiles_; }

    DISALLOW_IMPLICIT_CONSTRUCTORS(FixedTerrain);
private:
    int id_ = 0;
    std::string name_;
    int tile_w_ = 0;
    int tile_h_ = 0;
    int max_h_tiles_ = 0;
    int max_v_tiles_ = 0;
    int script_id_ = 0;
    int *tiles_ = nullptr;
    IndexedLinker *linkers_ = nullptr;
    int linker_size_ = 0;
    IndexedEntity *entities_ = nullptr;
    int entity_size_ = 0;
    IndexedActor *actors_ = nullptr;
    int actor_size_ = 0;
}; // class FixedTerrain

inline int FixedTerrain::tile(int x_tile, int y_tile) const {
    DCHECK_GE(x_tile, 0);
    DCHECK_LT(x_tile, max_h_tiles_);

    DCHECK_GE(y_tile, 0);
    DCHECK_LT(y_tile, max_v_tiles_);
    return tiles_[y_tile * max_h_tiles_ + x_tile];
}

} // namespace utaha

#endif // UTAHA_FIXED_TERRAIN_H_
