#include "ui-component-factory.h"
#include "ui-style-collection.h"
#include "ui-flat-menu.h"
#include "gtest/gtest.h"

namespace utaha {

class UIComponentFactoryTest : public ::testing::Test {
public:
    virtual void SetUp() override {
        style_ = new UIStyleCollection();
        std::string err;
        style_->LoadFromFile("tests/001-styles-demo1.lua", &err);
        ASSERT_TRUE(err.empty()) << err;
        factory_ = CreateUIComponentStyleFactory(style_);
    }

    virtual void TearDown() override {
        delete factory_;
        factory_ = nullptr;

        delete style_;
        style_ = nullptr;
    }

protected:
    UIStyleCollection *style_ = nullptr;
    UIComponentFactory *factory_ = nullptr;
};

TEST_F(UIComponentFactoryTest, Sanity) {
    std::unique_ptr<const UIFlatMenu> menu(factory_->CreateFlatMenu("test.1"));
    ASSERT_TRUE(!!menu);

    EXPECT_EQ("test.1", menu->name());
    EXPECT_NE(0, menu->id());
    EXPECT_EQ(4, menu->padding_size());
    EXPECT_NE(nullptr, menu->font());
}

} // namespace utaha
