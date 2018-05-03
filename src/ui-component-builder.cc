#include "ui-component-builder.h"
#include "ui-component-factory.h"
#include "ui-flat-menu.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-button.h"
#include "ui-layout.h"
#include "lua-utils.h"
#include "glog/logging.h"

namespace utaha {

UIComponentBuilder::UIComponentBuilder(UIComponentFactory *factory, SDL_Window *window)
    : factory_(DCHECK_NOTNULL(factory))
    , window_(DCHECK_NOTNULL(window)) {
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

UIFlatButtonBuilder *UIComponentBuilder::BeginFlatButton(const char *name) {
    auto component = factory_->CreateFlatButton(name);
    if (!component) {
        LOG(ERROR) << "can not create <UIFlatButton>!";
        return nullptr;
    }
    return new UIFlatButtonBuilder(component, factory_);
}

UILayoutBuilder *UIComponentBuilder::BeginLayout() {
    auto layout = new UILayout(window_);
    return new UILayoutBuilder(layout);
}

/*static*/ int UIComponentBuilder::BindTo(lua_State *L) {
    luabridge::getGlobalNamespace(L)
        .beginNamespace(kLuaNamespace)
            .beginClass<UIComponentBuilder>("UIComponentBuilder")
                .addFunction("beginFlatMenuGroup", &UIComponentBuilder::BeginFlatMenuGroup)
                .addFunction("beginFlatMenu", &UIComponentBuilder::BeginFlatMenu)
                .addFunction("beginFlatButton", &UIComponentBuilder::BeginFlatButton)
                .addFunction("beginLayout", &UIComponentBuilder::BeginLayout)
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
            .beginClass<UIFlatButtonBuilder>("FlatButtonBuilder")
                .addFunction("x", &UIFlatButtonBuilder::LetX)
                .addFunction("y", &UIFlatButtonBuilder::LetY)
                .addFunction("w", &UIFlatButtonBuilder::LetW)
                .addFunction("h", &UIFlatButtonBuilder::LetH)
            .endClass()
            .beginClass<UILayoutBuilder>("LayoutBuilder")
                .addFunction("paddingSize", &UILayoutBuilder::LetPaddingSize)
                .addFunction("verticalAlignment", &UILayoutBuilder::LetVerticalAlignment)
                .addFunction("horizontalAligment", &UILayoutBuilder::LetHorizontalAligment)
                .addFunction("debugMode", &UILayoutBuilder::LetDebugMode)
                .addFunction("beginRow", &UILayoutBuilder::BeginRow)
                .addFunction("endLayout", &UILayoutBuilder::EndLayout)
            .endClass()
            .beginClass<UILayoutRowBuilder>("LayoutRowBuilder")
                .addFunction("add", &UILayoutRowBuilder::AddComponent)
                .addFunction("endRow", &UILayoutRowBuilder::EndRow)
            .endClass()
            .beginClass<UIComponent>("Component").endClass()
            .deriveClass<UIFlatMenuGroup, UIComponent>("FlatMenuGroup").endClass()
            .deriveClass<UIFlatMenu, UIComponent>("FlatMenu").endClass()
            .deriveClass<UIFlatButton, UIComponent>("FlatButton").endClass()
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
    auto row = layout_->AddRow(static_cast<UILayout::Alignment>(aligment));
    return new UILayoutRowBuilder(row, this);
}

UILayout *UILayoutBuilder::EndLayout() {
    auto result = layout_;
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UILayoutRowBuilder
////////////////////////////////////////////////////////////////////////////////
UILayoutRowBuilder *UILayoutRowBuilder::AddComponent(UIComponent *component) {
    row_->AddComponent(component);
    return this;
}

UILayoutBuilder *UILayoutRowBuilder::EndRow() {
    auto result = builder_;
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UIFlatButtonBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatButtonBuilder *UIFlatButtonBuilder::LetX(int x) {
    component()->mutable_rect()->x = x;
    return this;
}

UIFlatButtonBuilder *UIFlatButtonBuilder::LetY(int y) {
    component()->mutable_rect()->y = y;
    return this;
}

UIFlatButtonBuilder *UIFlatButtonBuilder::LetW(int w) {
    component()->mutable_rect()->w = w;
    return this;
}

UIFlatButtonBuilder *UIFlatButtonBuilder::LetH(int h) {
    component()->mutable_rect()->h = h;
    return this;
}

UIFlatButton *UIFlatButtonBuilder::EndButton() {
    auto result = component();
    delete this;
    return result;
}

} // namespace utaha
