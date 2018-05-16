#include "ui-component-builder.h"
#include "ui-component-factory.h"
#include "ui-style-collection.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-menu.h"
#include "ui-form.h"
#include "ui-layout.h"
#include "lua-utils.h"
#include "gtest/gtest.h"
#include <SDL2/SDL.h>

namespace utaha {

class UIComponentBuilderTest : public ::testing::Test {
public:
    virtual void SetUp() override {
        style_ = new UIStyleCollection();
        std::string err;
        style_->LoadFromFile("tests/001-styles-demo1.lua", &err);
        ASSERT_TRUE(err.empty()) << err;
        factory_ = CreateUIComponentStyleFactory(style_);
        form_ = new UIForm();
        form_->OpenWindow("test", 1, 1);
        builder_ = new UIComponentBuilder(factory_, form_, form_);
    }

    virtual void TearDown() override {
        delete builder_;
        builder_ = nullptr;

        delete form_;
        form_ = nullptr;

        delete factory_;
        factory_ = nullptr;

        delete style_;
        style_ = nullptr;
    }

protected:
    UIStyleCollection *style_ = nullptr;
    UIComponentFactory *factory_ = nullptr;
    UIComponentBuilder *builder_ = nullptr;
    //SDL_Window *window_ = nullptr;
    UIForm *form_ = nullptr;
};

TEST_F(UIComponentBuilderTest, BuildFlatMenuGroup) {
    auto mg = builder_->BeginFlatMenuGroup("test")
        ->BeginColumn("test.column", 1)
            ->AddItem("test.1", 2, nullptr)
            ->AddDiv()
        ->EndColumn()
    ->EndMenuGroup();
    ASSERT_NE(nullptr, mg);
    ASSERT_EQ(1, mg->columns_size());
    ASSERT_EQ("test.column", mg->column(0).name);
}

TEST_F(UIComponentBuilderTest, BuildFlatMenuGroupFromFile) {
    auto L = LuaUtils::GeneralOpenLua();
    UIComponentBuilder::BindTo(L);

    luabridge::getGlobalNamespace(L)
    .beginNamespace(kLuaNamespace)
        .addVariable("uiBuilder", &builder_, false)
    .endNamespace();

    auto err = LuaUtils::ProtectedDoFile(L, "tests/002-flat-menu-group-builder.lua");
    ASSERT_EQ(nullptr, err) << err;

    std::unique_ptr<UIFlatMenuGroup> mg(luabridge::Stack<UIFlatMenuGroup *>::get(L, lua_gettop(L)));
    lua_close(L);

    ASSERT_TRUE(!!mg);
    ASSERT_EQ(1, mg->columns_size());
    ASSERT_NE(nullptr, mg->column(0).menu);

    auto menu = mg->column(0).menu;
    ASSERT_EQ(3, menu->items_size());
}

#define DEFINE_CMD_ID(M) \
    M(ID_TEST_1, 100) \
    M(ID_TEST_2, 200) \
    M(ID_TEST_3, 300)

struct TestR {
    enum ID: int {
        DEFINE_CMD_ID(LUA_DEF_CONST_ENUM)
    };
};

static const LuaConstantId test_id[] = {
    DEFINE_CMD_ID(LUA_DEF_CONST_ID)
};

TEST_F(UIComponentBuilderTest, BuildFlatMenuFromFile) {
    auto L = LuaUtils::GeneralOpenLua();
    UIComponentBuilder::BindTo(L);

    luabridge::getGlobalNamespace(L)
    .beginNamespace(kLuaNamespace)
        .addVariable("uiBuilder", &builder_, false)
    .endNamespace();

    LuaUtils::InitConstantId(L, kLuaNamespace, "R", test_id,
                             arraysize(test_id));

    auto err = LuaUtils::ProtectedDoFile(L, "tests/003-flat-menu-builder.lua");
    ASSERT_EQ(nullptr, err) << err;

    std::unique_ptr<UIFlatMenu> m(luabridge::Stack<UIFlatMenu *>::get(L, lua_gettop(L)));
    lua_close(L);

    ASSERT_TRUE(!!m);
    ASSERT_EQ(5, m->items_size());

    EXPECT_EQ("test.1", m->item(0).name);
    EXPECT_EQ(TestR::ID_TEST_1, m->item(0).cmd_id);

    EXPECT_EQ("test.2", m->item(2).name);
    EXPECT_EQ(TestR::ID_TEST_2, m->item(2).cmd_id);

    EXPECT_EQ("test.3", m->item(4).name);
    EXPECT_EQ(TestR::ID_TEST_3, m->item(4).cmd_id);
}

TEST_F(UIComponentBuilderTest, BuildLayoutFromFile) {
    auto L = LuaUtils::GeneralOpenLua();
    UIComponentBuilder::BindTo(L);

    luabridge::getGlobalNamespace(L)
    .beginNamespace(kLuaNamespace)
        .addVariable("uiBuilder", &builder_, false)
    .endNamespace();

    LuaUtils::InitConstantId(L, kLuaNamespace, "R", test_id,
                             arraysize(test_id));

    auto err = LuaUtils::ProtectedDoFile(L, "tests/004-layout-builder-1.lua");
    ASSERT_EQ(nullptr, err) << err;

    std::unique_ptr<const UILayout> layout(luabridge::Stack<UILayout *>::get(L, lua_gettop(L)));
    lua_close(L);
    ASSERT_TRUE(!!layout);
    EXPECT_EQ(1, layout->padding_size());
    EXPECT_TRUE(layout->debug_mode());
    EXPECT_EQ(UILayout::ALIGN_CENTER, layout->vertical_alignment());
    EXPECT_EQ(UILayout::ALIGN_CENTER, layout->horizontal_aligment());

    ASSERT_EQ(1, layout->rows_size());
    auto row = layout->row(0);
    EXPECT_EQ(UILayout::ALIGN_CENTER, row->horizontal_aligment());
}

} // namespace utaha
