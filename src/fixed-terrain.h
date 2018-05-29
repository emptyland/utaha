#ifndef UTAHA_FIXED_TERRAIN_H_
#define UTAHA_FIXED_TERRAIN_H_

#include "glog/logging.h"
#include "base.h"

namespace utaha {

struct IndexedTrap {
    int x;
    int y;
    int params;
    int script_id;
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
    DEF_VAL_PROP_RW(int, tile_w);
    DEF_VAL_PROP_RW(int, tile_h);
    DEF_VAL_PROP_RW(int, script_id);
    DEF_VAL_PROP_RW(int, max_h_tiles);
    DEF_VAL_PROP_RW(int, max_v_tiles);
    DEF_PTR_PROP_RW(int, tiles);
    DEF_PTR_PROP_RW(IndexedTrap, traps);
    DEF_VAL_PROP_RW(int, trap_size);
    DEF_PTR_PROP_RW(IndexedEntity, entities);
    DEF_VAL_PROP_RW(int, entity_size);
    DEF_PTR_PROP_RW(IndexedActor, actors);
    DEF_VAL_PROP_RW(int, actor_size);

    int tile_size() const { return max_h_tiles_ * max_v_tiles_; }
    inline int tile(int x_tile, int y_tile) const;

    const IndexedTrap *trap(int i) const {
        DCHECK_GE(i, 0);
        DCHECK_LT(i, trap_size_);
        return traps_ + i;
    }

    const IndexedEntity *entity(int i) const {
        DCHECK_GE(i, 0);
        DCHECK_LT(i, entity_size_);
        return entities_ + i;
    }

    const IndexedActor *actor(int i) const {
        DCHECK_GE(i, 0);
        DCHECK_LT(i, actor_size_);
        return actors_ + i;
    }

    DISALLOW_IMPLICIT_CONSTRUCTORS(FixedTerrain);
private:
    int id_ = 0;
    int tile_w_ = 0;
    int tile_h_ = 0;
    int max_h_tiles_ = 0;
    int max_v_tiles_ = 0;
    int script_id_ = 0;
    int *tiles_ = nullptr;
    IndexedTrap *traps_ = nullptr;
    int trap_size_ = 0;
    IndexedEntity *entities_ = nullptr;
    int entity_size_ = 0;
    IndexedActor *actors_ = nullptr;
    int actor_size_ = 0;
}; // class FixedTerrain

} // namespace utaha

#endif // UTAHA_FIXED_TERRAIN_H_
