#include "fixed-terrain.h"
#include "gtest/gtest.h"

namespace utaha {

TEST(FixedTerrainTest, Sanity) {
    FixedTerrain terrain;
    terrain.set_id(3000);
    terrain.set_max_h_tiles(256);
    terrain.set_max_v_tiles(128);
    ASSERT_EQ(256 * 128, terrain.tile_size());
}

} // namespace utaha
