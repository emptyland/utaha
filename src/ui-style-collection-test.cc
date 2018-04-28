#include "ui-style-collection.h"
#include "lua-utils.h"
#include "gtest/gtest.h"

namespace utaha {

class UIStyleCollectionTest : public ::testing::Test {
public:
    virtual void SetUp() override {
        L = utaha::LuaUtils::GeneralOpenLua();
        ASSERT_NE(nullptr, L);
        utaha::UIStyleCollection::BindTo(L);
    }

    virtual void TearDown() override {
        if (L) {
            lua_close(L);
        }
    }

protected:
    lua_State *L;
};

TEST_F(UIStyleCollectionTest, Sanity) {
    std::unique_ptr<utaha::UIStyleCollection> style(new utaha::UIStyleCollection());
    auto pt = style.get();
    luabridge::getGlobalNamespace(L)
        .beginNamespace(kLuaNamespace)
            .addVariable("styleCollection", &pt, false)
        .endNamespace();

    auto err = utaha::LuaUtils::ProtectedDoFile(L, "tests/000-style-loading.lua");
    EXPECT_EQ(0, err) << err;
    lua_close(L); L = nullptr;

    bool ok = true;
    ASSERT_NE(nullptr, style->FindFont("test.font", &ok));
    ASSERT_TRUE(ok);

    SDL_Color color;
    ASSERT_TRUE(style->FindColor("test.3", &color));
    EXPECT_EQ(0xff, color.r);
    EXPECT_EQ(0x01, color.g);
    EXPECT_EQ(0x02, color.b);
    EXPECT_EQ(0x00, color.a);

    ASSERT_EQ(1, style->FindInt("test.1", &ok));
    ASSERT_TRUE(ok);

    ASSERT_NEAR(1.1001f, style->FindFloat("test.4", &ok), 1e-6);
    ASSERT_TRUE(ok);

    auto rv = style->FindSize("test.2", &ok);
    ASSERT_TRUE(ok);
    ASSERT_EQ(100, std::get<0>(rv));
    ASSERT_EQ(200, std::get<1>(rv));
}

TEST_F(UIStyleCollectionTest, StyleDemo1) {

}

} // namespace utaha
