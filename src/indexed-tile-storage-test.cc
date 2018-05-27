#include "indexed-tile-storage.h"
#include "indexed-tile.h"
#include "base-io.h"
#include "gtest/gtest.h"

namespace utaha {

TEST(IndexedTileStorageTest, Store) {
    IndexedTileStorage storage(1000);

    storage.set_dir("tests/tiles");
    storage.set_grid_pic_name("tiles");
    bool ok = true;
    for (int i = 0; i < 100; ++i) {
        auto tile = new IndexedTile();
        tile->set_tex_id(i);
        if (i % 2 == 0) {
            tile->set_passable(IndexedTile::kWalkPass);
        } else {
            tile->set_passable(IndexedTile::kWalkPass | IndexedTile::kBulletPass);
        }
        ASSERT_GE(storage.PutTile(tile, &ok), 1000);
        ASSERT_TRUE(ok);
    }
    ASSERT_TRUE(storage.StoreToFile(GetFileSystem()));
}

TEST(IndexedTileStorageTest, Load) {
    IndexedTileStorage storage(1000);

    storage.set_dir("tests/tiles");
    ASSERT_TRUE(storage.LoadFromFile(GetFileSystem()));

    auto tile = storage.FindOrNull(1010);
    ASSERT_NE(nullptr, tile);
    EXPECT_EQ(1010, tile->id());
    EXPECT_EQ(1, tile->tex_id());
    
}

} // namespace utaha
