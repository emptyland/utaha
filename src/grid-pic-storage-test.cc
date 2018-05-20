#include "grid-pic-storage.h"
#include "gtest/gtest.h"
#include SDL_H
#include SDL_IMAGE_H

namespace utaha {

TEST(GridPicStorageTest, Store) {
    GridPicStorage storage;
    storage.set_dir("tests/grid");
    storage.set_name("test1");
    storage.set_grid_w(48);
    storage.set_grid_h(48);

    for (int i = 0; i < storage.pitch(); ++i) {
        auto grid = SDL_CreateRGBSurfaceWithFormat(0, storage.grid_w(),
                                                   storage.grid_h(), 32,
                                                   SDL_PIXELFORMAT_RGBA8888);
        ASSERT_NE(nullptr, grid) << SDL_GetError();
        SDL_LockSurface(grid);
        Uint32 *pixels = static_cast<Uint32 *>(grid->pixels);
        if (i % 2) {
            for (int ii = 0; ii < grid->w * grid->h; ++ii) {
                pixels[ii] = SDL_MapRGBA(grid->format, 0, 0, 0, 0xff);
            }
        } else {
            for (int ii = 0; ii < grid->w * grid->h; ++ii) {
                pixels[ii] = SDL_MapRGBA(grid->format, 0xf0, 0xf0, 0xf0, 0xff);
            }
        }
        SDL_UnlockSurface(grid);

        bool ok = true;
        storage.FindOrInsertGrid("[unknown]", i, grid, &ok);
        ASSERT_TRUE(ok);
    }

    ASSERT_TRUE(storage.StoreToFile());
}

TEST(GridPicStorageTest, Load) {
    GridPicStorage storage;
    storage.set_dir("tests/grid");
    storage.set_name("test1");
    storage.set_grid_w(48);
    storage.set_grid_h(48);

    ASSERT_TRUE(storage.LoadFromFile());
    EXPECT_EQ(storage.pitch(), storage.grid_pics().size());
}

} // namespace utaha
