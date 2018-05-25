#include "animated-avatar-storage.h"
#include "animated-avatar.h"
#include "gtest/gtest.h"

namespace utaha {

TEST(AnimatedAvatarStorageTest, Store) {
    AnimatedAvatarStorage storage(2000);

    storage.set_dir("tests/avatars");
    storage.set_grid_pic_name("avatar");
    for (int i = 0; i < 100; ++i) {
        auto avatar = new AnimatedAvatar;
        avatar->set_w(20);
        avatar->set_h(48);

        for (int j = 0; j < MAX_DIR; ++j) {
            for (int k = 0; k < AnimatedAvatar::Animation::kNumFrames; ++k) {
                avatar->set_frame(static_cast<Direction>(j), k, 2000 + k);
            }
        }
        bool ok = true;
        ASSERT_GE(storage.PutAvatar(avatar, &ok), 2000);
        ASSERT_TRUE(ok);
    }

    ASSERT_TRUE(storage.StoreToFile());
}

TEST(AnimatedAvatarStorageTest, Load) {
    AnimatedAvatarStorage storage(2000);
    storage.set_dir("tests/avatars");
    storage.set_grid_pic_name("avatar");

    ASSERT_TRUE(storage.LoadFromFile());

    auto avatar = storage.FindOrNull(2010);
    ASSERT_NE(nullptr, avatar);
    EXPECT_EQ(2010, avatar->id());
    EXPECT_EQ(2000, avatar->frame(DIR_S, 0));
    EXPECT_EQ(2001, avatar->frame(DIR_S, 1));
    EXPECT_EQ(2002, avatar->frame(DIR_S, 2));
}

} // namespace utaha
