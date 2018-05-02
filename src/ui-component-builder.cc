#include "ui-component-builder.h"
#include "ui-component-factory.h"
#include "ui-flat-menu.h"
#include "ui-flat-menu-group.h"
#include "ui-layout.h"
#include "lua-utils.h"
#include "glog/logging.h"

namespace utaha {

UIComponentBuilder::UIComponentBuilder(UIComponentFactory *factory)
    : factory_(DCHECK_NOTNULL(factory)) {
}

UIComponentBuilder::~UIComponentBuilder() {
}

UIFlatMenuGroupBuilder *UIComponentBuilder::BeginFlatMenuGroup(const char *name) {
    auto component = factory_->CreateFlatMenuGroup(name);
    if (!component) {
        LOG(ERROR) << "can not create <FlatMenuGroup>!";
        return nullptr;
    }
    return new UIFlatMenuGroupBuilder(component, factory_);
}

UIFlatMenuBuilder *UIComponentBuilder::BeginFlatMenu(const char *name) {
    auto component = factory_->CreateFlatMenu(name);
    if (!component) {
        LOG(ERROR) << "can not create <UIFlatMenuBuilder>!";
        return nullptr;
    }
    return new UIFlatMenuBuilder(component, factory_);
}

/*static*/ int UIComponentBuilder::BindTo(lua_State *L) {
    luabridge::getGlobalNamespace(L)
        .beginNamespace(kLuaNamespace)
            .beginClass<UIComponentBuilder>("UIComponentBuilder")
                .addFunction("beginFlatMenuGroup", &UIComponentBuilder::BeginFlatMenuGroup)
                .addFunction("beginFlatMenu", &UIComponentBuilder::BeginFlatMenu)
            .endClass()
            .beginClass<UIFlatMenuGroupBuilder>("FlatMenuGroupBuilder")
                .addFunction("beginColumn", &UIFlatMenuGroupBuilder::BeginColumn)
                .addFunction("endMenuGroup", &UIFlatMenuGroupBuilder::EndMenuGroup)
            .endClass()
            .beginClass<UIFlatMenuGroupColumnBuilder>("FlatMenuGroupColumnBuilder")
                .addFunction("addItem", &UIFlatMenuGroupColumnBuilder::AddItem)
                .addFunction("addDiv", &UIFlatMenuGroupColumnBuilder::AddDiv)
                .addFunction("endColumn", &UIFlatMenuGroupColumnBuilder::EndColumn)
            .endClass()
            .beginClass<UIFlatMenuBuilder>("FlatMenuBuilder")
                .addFunction("addItem", &UIFlatMenuBuilder::AddItem)
                .addFunction("addDiv", &UIFlatMenuBuilder::AddDiv)
                .addFunction("endMenu", &UIFlatMenuBuilder::EndMenu)
            .endClass()
            .beginClass<UILayoutBuilder>("LayoutBuilder")
                .addFunction("paddingSize", &UILayoutBuilder::LetPaddingSize)
                .addFunction("verticalAlignment", &UILayoutBuilder::LetVerticalAlignment)
                .addFunction("horizontalAligment", &UILayoutBuilder::LetHorizontalAligment)
                .addFunction("debugMode", &UILayoutBuilder::LetDebugMode)
            .endClass()
            .beginClass<UIFlatMenuGroup>("FlatMenuGroup").endClass()
            .beginClass<UIFlatMenu>("FlatMenu").endClass()
            .beginClass<UILayout>("Layout").endClass()
        .endNamespace();

    static const LuaConstantId alignments[] = {
        {"ALIGN_LEFT_OR_TOP",     UILayout::ALIGN_LEFT_OR_TOP},
        {"ALIGN_RIGHT_OR_BOTTOM", UILayout::ALIGN_RIGHT_OR_BOTTOM},
        {"ALIGN_CENTER",          UILayout::ALIGN_CENTER},
        {"ALIGN_FILL",            UILayout::ALIGN_FILL},
    };
    LuaUtils::InitConstantId(L, kLuaNamespace, "alignment", alignments, arraysize(alignments));
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// class UIFlatMenuGroupColumnBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatMenuGroupColumnBuilder *UIFlatMenuGroupBuilder::BeginColumn(const char *name, int cmd_id) {
    return new UIFlatMenuGroupColumnBuilder(name, cmd_id, nullptr, this);
}

UIFlatMenuGroup *UIFlatMenuGroupBuilder::EndMenuGroup() {
    auto result = component();
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UIFlatMenuGroupColumnBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatMenuGroupColumnBuilder::UIFlatMenuGroupColumnBuilder(const char *name,
                                                           int cmd_id,
                                                           void *param,
                                                           UIFlatMenuGroupBuilder *builder)
    : col_name_(DCHECK_NOTNULL(name))
    , col_cmd_id_(cmd_id)
    , col_param_(param)
    , builder_(DCHECK_NOTNULL(builder))
    , UIComponentBuilderBase(builder->factory()->CreateFlatMenu(name), builder->factory()) {
}

UIFlatMenuGroupColumnBuilder *
UIFlatMenuGroupColumnBuilder::AddItem(const char *name, int cmd_id, void *param) {
    component()->AddItem(name, cmd_id, param);
    return this;
}

UIFlatMenuGroupColumnBuilder *UIFlatMenuGroupColumnBuilder::AddDiv() {
    component()->AddDiv();
    return this;
}

UIFlatMenuGroupBuilder *UIFlatMenuGroupColumnBuilder::EndColumn() {
    auto result = builder_;
    result->component()->AddColumn(col_name_.c_str(), col_cmd_id_, component());
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UIFlatMenuBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatMenuBuilder *UIFlatMenuBuilder::AddItem(const char *name, int cmd_id,
                                              void *param) {
    component()->AddItem(name, cmd_id, param);
    return this;
}

UIFlatMenuBuilder *UIFlatMenuBuilder::AddDiv() {
    component()->AddDiv();
    return this;
}

UIFlatMenu *UIFlatMenuBuilder::EndMenu() {
    auto result = component();
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UILayoutBuilder
////////////////////////////////////////////////////////////////////////////////

UILayoutBuilder *UILayoutBuilder::LetPaddingSize(int padding_size) {
    layout_->set_padding_size(padding_size);
    return this;
}

UILayoutBuilder *UILayoutBuilder::LetVerticalAlignment(int aligment) {
    layout_->set_vertical_alignment(static_cast<UILayout::Alignment>(aligment));
    return this;
}

UILayoutBuilder *UILayoutBuilder::LetHorizontalAligment(int aligment) {
    layout_->set_horizontal_aligment(static_cast<UILayout::Alignment>(aligment));
    return this;
}

UILayoutBuilder *UILayoutBuilder::LetDebugMode(bool turn_on) {
    layout_->set_debug_mode(turn_on);
    return this;
}

UILayoutRowBuilder *UILayoutBuilder::BeginRow(int aligment) {
    return nullptr;
}

UILayout *UILayoutBuilder::EndLayout() {
    return layout_;
}

} // namespace utaha
