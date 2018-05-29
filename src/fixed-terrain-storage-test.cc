#include "fixed-terrain-storage.h"
#include "base-io.h"
#include "gtest/gtest.h"

namespace utaha {


TEST(FixedTerrainStorageTest, StoreTest1) {
    FixedTerrainStorage storage(3000);
    storage.set_dir("tests/terrain/test1");
    storage.set_tex_name("tile");

    FixedTerrain *terrain = new FixedTerrain();
    terrain->set_name("main");
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

    ASSERT_TRUE(storage.LoadFromFile(GetFileSystem()));
    EXPECT_EQ("tile", storage.tex_name());
    EXPECT_EQ(3010, storage.next_id());
    ASSERT_EQ(1, storage.entity_size());

    auto terrain = storage.FindOrNull(3000);
    ASSERT_NE(nullptr, terrain);
    EXPECT_EQ("main", terrain->name());
    EXPECT_EQ(48, terrain->tile_w());
    EXPECT_EQ(48, terrain->tile_h());
    EXPECT_EQ(32, terrain->max_h_tiles());
    EXPECT_EQ(32, terrain->max_v_tiles());
    EXPECT_EQ(100, terrain->script_id());

    for (int x = 0; x < terrain->max_h_tiles(); ++x) {
        for (int y = 0; y < terrain->max_v_tiles(); ++y) {
            ASSERT_EQ(22, terrain->tile(x, y)) << "x=" << x << ", y=" << y;
        }
    }
}

TEST(FixedTerrainStorageTest, StoreTest2) {
    FixedTerrainStorage storage(3000);
    storage.set_dir("tests/terrain/test2");
    storage.set_tex_name("tile");

    for (int i = 0; i < 32; ++i) {
        FixedTerrain *terrain = new FixedTerrain();
        terrain->set_name(Original::sprintf("main-%02d", i));
        terrain->set_tile_h(48);
        terrain->set_tile_w(48);
        terrain->set_max_v_tiles(32);
        terrain->set_max_h_tiles(32);
        terrain->set_script_id(100 + i);

        auto tiles = new int[32 * 32];
        for (int j = 0; j < terrain->tile_size(); ++j) {
            tiles[j] = i;
        }
        terrain->set_tiles(tiles);

        bool ok = true;
        auto terrain_id = storage.Put(terrain, &ok);
        ASSERT_LE(storage.start_id(), terrain_id);
        ASSERT_TRUE(ok);
    }
    ASSERT_TRUE(storage.StoreToFile(GetFileSystem()));
}

TEST(FixedTerrainStorageTest, LoadTest2) {
    FixedTerrainStorage storage(3000);
    storage.set_dir("tests/terrain/test2");

    ASSERT_TRUE(storage.LoadFromFile(GetFileSystem()));
    EXPECT_EQ("tile", storage.tex_name());
    EXPECT_EQ(3000 + 32 * 10, storage.next_id());
    ASSERT_EQ(32, storage.entity_size());

    for (int i = 0; i < 32; ++i) {
        auto terrain = storage.FindOrNull(3000 + i * 10);
        ASSERT_NE(nullptr, terrain);
        EXPECT_EQ(Original::sprintf("main-%02d", i), terrain->name());
        EXPECT_EQ(48, terrain->tile_w());
        EXPECT_EQ(48, terrain->tile_h());
        EXPECT_EQ(32, terrain->max_h_tiles());
        EXPECT_EQ(32, terrain->max_v_tiles());
        EXPECT_EQ(100 + i, terrain->script_id());

        for (int x = 0; x < terrain->max_h_tiles(); ++x) {
            for (int y = 0; y < terrain->max_v_tiles(); ++y) {
                ASSERT_EQ(i, terrain->tile(x, y))
                    << "id=" << terrain->id()
                    << " x=" << x << ", y=" << y;
            }
        }
    }
}

TEST(FixedTerrainStorageTest, StoreTest3) {
    FixedTerrainStorage storage(3000);
    storage.set_dir("tests/terrain/test3");
    storage.set_tex_name("tile");

    FixedTerrain *terrain = new FixedTerrain();
    terrain->set_name("main");
    terrain->set_tile_h(48);
    terrain->set_tile_w(48);
    terrain->set_max_v_tiles(32);
    terrain->set_max_h_tiles(32);
    terrain->set_script_id(100);

    auto tiles = new int[32 * 32];
    for (int i = 0; i < terrain->tile_size(); ++i) {
        tiles[i] = 33;
    }
    terrain->set_tiles(tiles);

    terrain->set_linker_size(3);
    auto linkers = new IndexedLinker[terrain->linker_size()];
    for (int i = 0; i < terrain->linker_size(); ++i) {
        linkers[i].x = 1;
        linkers[i].y = 2;
        linkers[i].to_id = 3300;
        linkers[i].to_x = 3;
        linkers[i].to_y = 4;
    }
    terrain->set_linkers(linkers);

    terrain->set_entity_size(5);
    auto entities = new IndexedEntity[terrain->entity_size()];
    for (int i = 0; i < terrain->entity_size(); ++i) {
        entities[i].x = 22;
        entities[i].y = 11;
        entities[i].spirit_id = 1000;
    }
    terrain->set_entities(entities);

    terrain->set_actor_size(7);
    auto actors = new IndexedActor[terrain->actor_size()];
    for (int i = 0; i < terrain->actor_size(); ++i) {
        actors[i].x = 33;
        actors[i].y = 44;
        actors[i].lv = 1;
        actors[i].param = -1;
        actors[i].actor_id = 2000;
    }
    terrain->set_actors(actors);

    bool ok = true;
    storage.Put(terrain, &ok);
    ASSERT_TRUE(ok);
    EXPECT_EQ(3010, storage.next_id());

    ASSERT_TRUE(storage.StoreToFile(GetFileSystem()));
}

TEST(FixedTerrainStorageTest, LoadTest3) {
    FixedTerrainStorage storage(3000);
    storage.set_dir("tests/terrain/test3");
    storage.set_tex_name("tile");

    ASSERT_TRUE(storage.LoadFromFile(GetFileSystem()));
    EXPECT_EQ("tile", storage.tex_name());
    EXPECT_EQ(3010, storage.next_id());
    ASSERT_EQ(1, storage.entity_size());

    auto terrain = storage.FindOrNull(3000);
    ASSERT_NE(nullptr, terrain);
    ASSERT_EQ(3, terrain->linker_size());
    for (int i = 0; i < terrain->linker_size(); ++i) {
        EXPECT_EQ(1, terrain->linker(i).x);
        EXPECT_EQ(2, terrain->linker(i).y);
        EXPECT_EQ(3300, terrain->linker(i).to_id);
        EXPECT_EQ(3, terrain->linker(i).to_x);
        EXPECT_EQ(4, terrain->linker(i).to_y);
    }

    ASSERT_EQ(5, terrain->entity_size());
    for (int i = 0; i < terrain->entity_size(); ++i) {
        EXPECT_EQ(22, terrain->entity(i).x);
        EXPECT_EQ(11, terrain->entity(i).y);
        EXPECT_EQ(1000, terrain->entity(i).spirit_id);
    }

    ASSERT_EQ(7, terrain->actor_size());
    for (int i = 0; i < terrain->actor_size(); ++i) {
        EXPECT_EQ(33, terrain->actor(i).x);
        EXPECT_EQ(44, terrain->actor(i).y);
        EXPECT_EQ(1, terrain->actor(i).lv);
        EXPECT_EQ(-1, terrain->actor(i).param);
        EXPECT_EQ(2000, terrain->actor(i).actor_id);
    }
}

} // namespace utaha
