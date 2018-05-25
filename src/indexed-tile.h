#ifndef UTAHA_INDEXED_TILE_H_
#define UTAHA_INDEXED_TILE_H_

#include "base.h"
#include <stdint.h>
#include <string>

namespace utaha {

class IndexedTile {
public:
    constexpr static const int kWalkPass   = 1u;
    constexpr static const int kFlyPass    = 1u << 1;
    constexpr static const int kBulletPass = 1u << 2;
    constexpr static const int kMagicPass  = 1u << 3;

    IndexedTile() = default;
    ~IndexedTile() = default;

    DEF_VAL_PROP_RW(int, id);
    DEF_VAL_PROP_RW(std::string, name);
    DEF_VAL_PROP_RW(int, tex_id);
    DEF_VAL_PROP_RW(uint32_t, passable);

    bool walk_pass() const { return passable_ & kWalkPass; }
    bool fly_pass() const { return passable_ & kFlyPass; }
    bool bullet_pass() const { return passable_ & kBulletPass; }
    bool magic_pass() const { return passable_ & kMagicPass; }

    inline void CopyTo(IndexedTile *other) const;

    DISALLOW_IMPLICIT_CONSTRUCTORS(IndexedTile);
private:
    int id_ = 0;
    std::string name_;
    int tex_id_ = 0;
    uint32_t passable_ = 0;
}; // class IndexedTile

inline void IndexedTile::CopyTo(IndexedTile *other) const {
    other->set_id(id_);
    other->set_name(name_);
    other->set_tex_id(tex_id_);
    other->set_passable(passable_);
}

} // namespace utaha

#endif // UTAHA_INDEXED_TILE_H_
