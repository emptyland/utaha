#include "raw-pic-collection.h"
#include "gtest/gtest.h"

namespace utaha {

TEST(RawPicCollectionTest, Sanity) {
    RawPicCollection pics;

    std::string err;
    EXPECT_EQ(3, pics.LoadWithBootstrapScript("tests/006-raw-pic-load.lua",
                                              &err));
    ASSERT_TRUE(err.empty()) << err;

    auto pic = pics.FindPicOrNull("mon1.png");
    ASSERT_NE(nullptr, pic);
}

} // namespace utaha
