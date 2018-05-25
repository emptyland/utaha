#include "animated-avatar.h"
#include "gtest/gtest.h"

namespace utaha {

TEST(AnimatedAvatarTest, Sanity) {
    AnimatedAvatar avatar;
    avatar.mutable_animation(DIR_S)->set_frame(0, 1000);
    avatar.mutable_animation(DIR_S)->set_frame(1, 2000);

    EXPECT_EQ(1000, avatar.animation(DIR_S).frame(0));
    EXPECT_EQ(2000, avatar.animation(DIR_S).frame(1));
    EXPECT_EQ(1000, avatar.frame(DIR_S, 0));
    EXPECT_EQ(2000, avatar.frame(DIR_S, 1));
}

} // namespace utaha
