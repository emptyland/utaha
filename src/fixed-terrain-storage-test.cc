#include "fixed-terrain-storage.h"
#include "base-io.h"
#include "gtest/gtest.h"

namespace utaha {

//class FixedTerrainStorageTest : public ::testing::Test {
//public:
//    virtual void SetUp() override {
//        storage_ = new FixedTerrainStorage(3000);
//
//    }
//
//    virtual void TearDown() override {
//
//    }
//
//protected:
//    FixedTerrainStorage *storage_ = nullptr;
//};

TEST(FixedTerrainStorageTest, StoreTest1) {
    FixedTerrainStorage storage(3000);
    storage.set_dir("tests/terrain/test1");
    storage.set_tex_name("tile");

    FixedTerrain *terrain = new FixedTerrain();
    terrain->set_tile_h(48);
    terrain->set_tile_w(48);
    terrain->set_max_v_tiles(32);
    terrain->set_max_h_tiles(32);
    terrain->set_script_id(100);

    auto tiles = new int[32 * 32];
    for (int i = 0; i < terrain->tile_size(); ++i) {
        tiles[i] = 22;
    }
    terrain->set_tiles(tiles);

    bool ok = true;
    auto terrain_id = storage.Put(terrain, &ok);
    ASSERT_TRUE(ok);
    EXPECT_EQ(3010, storage.next_id());

    ASSERT_TRUE(storage.StoreToFile(GetFileSystem()));

    ASSERT_EQ(terrain, storage.FindOrNull(terrain_id));
}

TEST(FixedTerrainStorageTest, LoadTest1) {
    FixedTerrainStorage storage(3000);
    storage.set_dir("tests/terrain/test1");
    storage.set_tex_name("tile");

    ASSERT_TRUE(storage.LoadFromFile(GetFileSystem()));
    EXPECT_EQ(3010, storage.next_id());
    ASSERT_EQ(1, storage.entity_size());

    auto terrain = storage.FindOrNull(3000);
    ASSERT_NE(nullptr, terrain);
    EXPECT_EQ(48, terrain->tile_w());
    EXPECT_EQ(48, terrain->tile_h());
    EXPECT_EQ(32, terrain->max_h_tiles());
    EXPECT_EQ(32, terrain->max_v_tiles());
    EXPECT_EQ(100, terrain->script_id());
}

} // namespace utaha
