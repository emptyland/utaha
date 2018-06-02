#include "ui-component-builder.h"
#include "ui-component-factory.h"
#include "ui-flat-menu.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-button.h"
#include "ui-flat-input-box.h"
#include "ui-flat-check-box.h"
#include "ui-flat-status-bar.h"
#include "ui-flat-label.h"
#include "ui-flat-pic-view.h"
#include "ui-flat-text-box.h"
#include "ui-pic-grid-selector.h"
#include "ui-animated-avatar-view.h"
#include "ui-terrain-view.h"
#include "ui-layout.h"
#include "lua-utils.h"
#include "glog/logging.h"

namespace utaha {

UIComponentBuilder::UIComponentBuilder(UIComponentFactory *factory,
                                       UIForm *form,
                                       InteractiveListenner *listenner)
    : factory_(DCHECK_NOTNULL(factory))
    , form_(DCHECK_NOTNULL(form))
    , listenner_(DCHECK_NOTNULL(listenner)) {
}

UIComponentBuilder::~UIComponentBuilder() {
}

UIFlatMenuGroupBuilder *
UIComponentBuilder::BeginFlatMenuGroup(const char *name) {
    auto component = factory_->CreateFlatMenuGroup(name);
    if (!component) {
        LOG(ERROR) << "Can not create <FlatMenuGroup>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UIFlatMenuGroupBuilder(listenner_, component, factory_);
}

UIFlatMenuBuilder *UIComponentBuilder::BeginFlatMenu(const char *name) {
    auto component = factory_->CreateFlatMenu(name);
    if (!component) {
        LOG(ERROR) << "Can not create <UIFlatMenuBuilder>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UIFlatMenuBuilder(component, factory_);
}

UIFlatButtonBuilder *UIComponentBuilder::BeginFlatButton(const char *name) {
    auto component = factory_->CreateFlatButton(name);
    if (!component) {
        LOG(ERROR) << "Can not create <UIFlatButton>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UIFlatButtonBuilder(component, factory_);
}

UIFlatInputBoxBuilder *UIComponentBuilder::BeginFlatInputBox(const char *name) {
    auto component = factory_->CreateFlatInputBox(name);
    if (!component) {
        LOG(ERROR) << "Can not create <FlatInputBox>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UIFlatInputBoxBuilder(component, factory_);
}

UIFlatLabelBuilder *UIComponentBuilder::BeginFlatLabel(const char *name) {
    auto component = factory_->CreateFlatLabel(name);
    if (!component) {
        LOG(ERROR) << "Can not create <FlatLabel>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UIFlatLabelBuilder(component, factory_);
}

UIFlatLabel *UIComponentBuilder::LetFlatLabel(const char *name,
                                              const char *text) {
    auto component = factory_->CreateFlatLabel(name);
    if (!component) {
        LOG(ERROR) << "Can not create <FlatLabel>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    component->set_text(text);
    return component;
}

UIFlatPicView *UIComponentBuilder::LetFlatPicView(const char *name) {
    auto component = factory_->CreateFlatPicView(name);
    if (!component) {
        LOG(ERROR) << "Can not create <FlatPicView>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return component;
}

UIFlatCheckBoxBuilder *UIComponentBuilder::BeginFlatCheckBox(const char *name) {
    auto component = factory_->CreateFlatCheckBox(name);
    if (!component) {
        LOG(ERROR) << "Can not create <UIFlatCheckBox>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UIFlatCheckBoxBuilder(component, factory_);
}

UIFlatStatusBarBuilder *
UIComponentBuilder::BeginFlatStatusBar(const char *name) {
    auto component = factory_->CreateFlatStatusBar(name);
    if (!component) {
        LOG(ERROR) << "Can not create <UIFlatStatusBar>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UIFlatStatusBarBuilder(component, factory_);
}

UIFlatTextBoxBuilder *UIComponentBuilder::BeginFlatTextBox(const char *name) {
    auto component = factory_->CreateFlatTextBox(name);
    if (!component) {
        LOG(ERROR) << "Can not create <UIFlatTextBox>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UIFlatTextBoxBuilder(component, factory_);
}

UIPicGridSelectorBuilder *
UIComponentBuilder::BeginPicGridSelector(const char *name) {
    auto component = factory_->CreatePicGridSelector(name);
    if (!component) {
        LOG(ERROR) << "Can not create <UIPicGridSelector>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UIPicGridSelectorBuilder(component, factory_);
}

UIAnimatedAvatarViewBuilder *
UIComponentBuilder::BeginAnimatedAvatarView(const char *name) {
    auto component = factory_->CreateAnimatedAvatarView(name);
    if (!component) {
        LOG(ERROR) << "Can not create <UIAnimatedAvatarView>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UIAnimatedAvatarViewBuilder(component, factory_);
}

UITerrainViewBuilder *UIComponentBuilder::BeginTerrainView(const char *name) {
    auto component = factory_->CreateTerrainView(name);
    if (!component) {
        LOG(ERROR) << "Can not create <UITerrainView>!";
        return nullptr;
    }
    component->AddListenner(listenner_);
    return new UITerrainViewBuilder(component, factory_);
}

UILayoutBuilder *UIComponentBuilder::BeginLayout() {
    auto layout = new UILayout(form_);
    return new UILayoutBuilder(layout);
}

/*static*/ int UIComponentBuilder::BindTo(lua_State *L) {
    luabridge::getGlobalNamespace(L)
        .beginNamespace(kLuaNamespace)
            .beginClass<UIComponentBuilder>("UIComponentBuilder")
                .addFunction("beginFlatMenuGroup",
                             &UIComponentBuilder::BeginFlatMenuGroup)
                .addFunction("beginFlatMenu",
                             &UIComponentBuilder::BeginFlatMenu)
                .addFunction("beginFlatButton",
                             &UIComponentBuilder::BeginFlatButton)
                .addFunction("beginFlatInputBox",
                             &UIComponentBuilder::BeginFlatInputBox)
                .addFunction("beginFlatLabel",
                             &UIComponentBuilder::BeginFlatLabel)
                .addFunction("flatLabel", &UIComponentBuilder::LetFlatLabel)
                .addFunction("flatPicView", &UIComponentBuilder::LetFlatPicView)
                .addFunction("beginFlatCheckBox",
                             &UIComponentBuilder::BeginFlatCheckBox)
                .addFunction("beginFlatStatusBar",
                             &UIComponentBuilder::BeginFlatStatusBar)
                .addFunction("beginFlatTextBox",
                             &UIComponentBuilder::BeginFlatTextBox)
                .addFunction("beginPicGridSelector",
                             &UIComponentBuilder::BeginPicGridSelector)
                .addFunction("beginAnimatedAvatarView",
                             &UIComponentBuilder::BeginAnimatedAvatarView)
                .addFunction("beginTerrainView",
                             &UIComponentBuilder::BeginTerrainView)
                .addFunction("beginLayout", &UIComponentBuilder::BeginLayout)
            .endClass()
            .beginClass<UIFlatMenuGroupBuilder>("FlatMenuGroupBuilder")
                .addFunction("beginColumn",
                             &UIFlatMenuGroupBuilder::BeginColumn)
                .addFunction("endMenuGroup",
                             &UIFlatMenuGroupBuilder::EndMenuGroup)
            .endClass()
            .beginClass<UIFlatMenuGroupColumnBuilder>(
                "FlatMenuGroupColumnBuilder")
                .addFunction("addItem", &UIFlatMenuGroupColumnBuilder::AddItem)
                .addFunction("addDiv", &UIFlatMenuGroupColumnBuilder::AddDiv)
                .addFunction("endColumn",
                             &UIFlatMenuGroupColumnBuilder::EndColumn)
            .endClass()
            .beginClass<UIFlatMenuBuilder>("FlatMenuBuilder")
                .addFunction("addItem", &UIFlatMenuBuilder::AddItem)
                .addFunction("addDiv", &UIFlatMenuBuilder::AddDiv)
                .addFunction("endMenu", &UIFlatMenuBuilder::EndMenu)
            .endClass()
            .beginClass<UIFlatButtonBuilder>("FlatButtonBuilder")
                .addFunction("text", &UIFlatButtonBuilder::LetText)
                .addFunction("cmdId", &UIFlatButtonBuilder::LetCmdId)
                .addFunction("x", &UIFlatButtonBuilder::LetX)
                .addFunction("y", &UIFlatButtonBuilder::LetY)
                .addFunction("w", &UIFlatButtonBuilder::LetW)
                .addFunction("h", &UIFlatButtonBuilder::LetH)
                .addFunction("endButton", &UIFlatButtonBuilder::EndButton)
            .endClass()
            .beginClass<UIFlatInputBoxBuilder>("FlatInputBoxBuilder")
                .addFunction("text", &UIFlatInputBoxBuilder::LetText)
                .addFunction("maxInput", &UIFlatInputBoxBuilder::LetMaxInput)
                .addFunction("x", &UIFlatInputBoxBuilder::LetX)
                .addFunction("y", &UIFlatInputBoxBuilder::LetY)
                .addFunction("w", &UIFlatInputBoxBuilder::LetW)
                .addFunction("h", &UIFlatInputBoxBuilder::LetH)
                .addFunction("endInputBox", &UIFlatInputBoxBuilder::EndInputBox)
            .endClass()
            .beginClass<UIFlatLabelBuilder>("FlatLabelBuilder")
                .addFunction("text", &UIFlatLabelBuilder::LetText)
                .addFunction("fontColor", &UIFlatLabelBuilder::LetFontColor)
                .addFunction("bgColor", &UIFlatLabelBuilder::LetBgColor)
                .addFunction("endLabel", &UIFlatLabelBuilder::EndLabel)
            .endClass()
            .beginClass<UIFlatCheckBoxBuilder>("FlatCheckBoxBuilder")
                .addFunction("label", &UIFlatCheckBoxBuilder::LetLabel)
                .addFunction("value", &UIFlatCheckBoxBuilder::LetValue)
                .addFunction("x", &UIFlatCheckBoxBuilder::LetX)
                .addFunction("y", &UIFlatCheckBoxBuilder::LetY)
                .addFunction("w", &UIFlatCheckBoxBuilder::LetW)
                .addFunction("h", &UIFlatCheckBoxBuilder::LetH)
                .addFunction("endCheckBox", &UIFlatCheckBoxBuilder::EndCheckBox)
            .endClass()
            .beginClass<UIFlatStatusBarBuilder>("FlatStatusBarBuilder")
                .addFunction("addGrid", &UIFlatStatusBarBuilder::BeginGrid)
                .addFunction("x", &UIFlatStatusBarBuilder::LetX)
                .addFunction("y", &UIFlatStatusBarBuilder::LetY)
                .addFunction("w", &UIFlatStatusBarBuilder::LetW)
                .addFunction("h", &UIFlatStatusBarBuilder::LetH)
                .addFunction("endStatusBar",
                             &UIFlatStatusBarBuilder::EndStatusBar)
            .endClass()
            .beginClass<UIFlatStatusBarGridBuilder>("FlatStatusBarGridBuilder")
                .addFunction("text", &UIFlatStatusBarGridBuilder::LetText)
                .addFunction("w", &UIFlatStatusBarGridBuilder::LetW)
                .addFunction("fontColor",
                             &UIFlatStatusBarGridBuilder::LetFontColor)
                .addFunction("bgColor", &UIFlatStatusBarGridBuilder::LetBgColor)
                .addFunction("endGrid", &UIFlatStatusBarGridBuilder::EndGrid)
            .endClass()
            .beginClass<UIFlatTextBoxBuilder>("FlatTextBoxBuilder")
                .addFunction("x", &UIFlatTextBoxBuilder::LetX)
                .addFunction("y", &UIFlatTextBoxBuilder::LetY)
                .addFunction("w", &UIFlatTextBoxBuilder::LetW)
                .addFunction("h", &UIFlatTextBoxBuilder::LetH)
                .addFunction("endTextBox", &UIFlatTextBoxBuilder::EndTextBox)
            .endClass()
            .beginClass<UIPicGridSelectorBuilder>("PicGridSelectorBuilder")
                .addFunction("cmdId", &UIPicGridSelectorBuilder::LetCmdId)
                .addFunction("gridSizeW",
                             &UIPicGridSelectorBuilder::LetGridSizeW)
                .addFunction("gridSizeH",
                             &UIPicGridSelectorBuilder::LetGridSizeH)
                .addFunction("endPicGridSelectorFromFile",
                    &UIPicGridSelectorBuilder::EndPicGridSelectorFromFile)
                .addFunction("endPicGridSelector",
                             &UIPicGridSelectorBuilder::EndPicGridSelector)
            .endClass()
            .beginClass<UIAnimatedAvatarViewBuilder>("AnimatedAvatarBuilder")
                .addFunction("viewW", &UIAnimatedAvatarViewBuilder::LetViewW)
                .addFunction("viewH", &UIAnimatedAvatarViewBuilder::LetViewH)
                .addFunction("animatedSpeed",
                             &UIAnimatedAvatarViewBuilder::LetAnimatedSpeed)
                .addFunction("endAnimatedAvatarView",
                             &UIAnimatedAvatarViewBuilder::EndAnimatedAvatarView)
            .endClass()
            .beginClass<UITerrainViewBuilder>("TerrainViewBuilder")
                .addFunction("cmdId", &UITerrainViewBuilder::LetCmdId)
                .addFunction("hasRuler", &UITerrainViewBuilder::LetHasRuler)
                .addFunction("tileW", &UITerrainViewBuilder::LetTileW)
                .addFunction("tileH", &UITerrainViewBuilder::LetTileH)
                .addFunction("maxHTiles", &UITerrainViewBuilder::LetMaxHTiles)
                .addFunction("maxVTiles", &UITerrainViewBuilder::LetMaxVTiles)
                .addFunction("viewPortHTiles",
                             &UITerrainViewBuilder::LetViewPortHTiles)
                .addFunction("viewPortVTiles",
                             &UITerrainViewBuilder::LetViewPortVTiles)
                .addFunction("scrollingSpeed",
                             &UITerrainViewBuilder::LetScrollingSpeed)
                .addFunction("x", &UITerrainViewBuilder::LetX)
                .addFunction("y", &UITerrainViewBuilder::LetY)
                .addFunction("w", &UITerrainViewBuilder::LetW)
                .addFunction("h", &UITerrainViewBuilder::LetH)
                .addFunction("endTerrainView",
                             &UITerrainViewBuilder::EndTerrainView)
            .endClass()
            .beginClass<UILayoutBuilder>("LayoutBuilder")
                .addFunction("paddingSize", &UILayoutBuilder::LetPaddingSize)
                .addFunction("verticalAlignment",
                             &UILayoutBuilder::LetVerticalAlignment)
                .addFunction("horizontalAligment",
                             &UILayoutBuilder::LetHorizontalAligment)
                .addFunction("debugMode", &UILayoutBuilder::LetDebugMode)
                .addFunction("beginRow", &UILayoutBuilder::BeginRow)
                .addFunction("endLayout", &UILayoutBuilder::EndLayout)
            .endClass()
            .beginClass<UILayoutRowBuilder>("LayoutRowBuilder")
                .addFunction("add", &UILayoutRowBuilder::AddComponent)
                .addFunction("endRow", &UILayoutRowBuilder::EndRow)
            .endClass()
            .beginClass<UIComponent>("Component").endClass()
            .deriveClass<UIFlatMenuGroup, UIComponent>("FlatMenuGroup")
            .endClass()
            .deriveClass<UIFlatMenu, UIComponent>("FlatMenu").endClass()
            .deriveClass<UIFlatButton, UIComponent>("FlatButton").endClass()
            .deriveClass<UIFlatInputBox, UIComponent>("FlatInputBox").endClass()
            .deriveClass<UIFlatLabel, UIComponent>("FlatLabel").endClass()
            .deriveClass<UIFlatCheckBox, UIComponent>("FlatCheckBox")
            .endClass()
            .deriveClass<UIFlatStatusBar, UIComponent>("FlatStatusBar")
            .endClass()
            .deriveClass<UIFlatPicView, UIComponent>("FlatPicView").endClass()
            .deriveClass<UIPicGridSelector, UIComponent>("PicGridSelector")
            .endClass()
            .deriveClass<UIAnimatedAvatarView, UIComponent>("AnimatedAvatarView")
            .endClass()
            .deriveClass<UITerrainView, UIComponent>("TerrainView").endClass()
            .deriveClass<UIFlatTextBox, UIComponent>("FlatTextBox").endClass()
            .beginClass<UILayout>("Layout").endClass()
        .endNamespace();

    static const LuaConstantId alignments[] = {
        {"ALIGN_LEFT_OR_TOP",     UILayout::ALIGN_LEFT_OR_TOP},
        {"ALIGN_RIGHT_OR_BOTTOM", UILayout::ALIGN_RIGHT_OR_BOTTOM},
        {"ALIGN_CENTER",          UILayout::ALIGN_CENTER},
        {"ALIGN_FILL",            UILayout::ALIGN_FILL},
    };
    LuaUtils::InitConstantId(L, kLuaNamespace, "alignment", alignments,
                             arraysize(alignments));
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// class UIFlatMenuGroupColumnBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatMenuGroupColumnBuilder *
UIFlatMenuGroupBuilder::BeginColumn(const char *name, int cmd_id) {
    return new UIFlatMenuGroupColumnBuilder(name, cmd_id, nullptr, listenner_,
                                            this);
}

UIFlatMenuGroup *UIFlatMenuGroupBuilder::EndMenuGroup() {
    auto result = component();
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UIFlatMenuGroupColumnBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatMenuGroupColumnBuilder::UIFlatMenuGroupColumnBuilder(
        const char *name,
        int cmd_id,
        void *param,
        InteractiveListenner *listenner,
        UIFlatMenuGroupBuilder *builder)
    : col_name_(DCHECK_NOTNULL(name))
    , col_cmd_id_(cmd_id)
    , col_param_(param)
    , builder_(DCHECK_NOTNULL(builder))
    , UIComponentBuilderBase(builder->factory()->CreateFlatMenu(name),
                             builder->factory()) {
    component()->AddListenner(listenner);
}

UIFlatMenuGroupColumnBuilder *
UIFlatMenuGroupColumnBuilder::AddItem(const char *name, int cmd_id,
                                      void *param) {
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

UIFlatButtonBuilder *UIFlatButtonBuilder::LetText(const char *text) {
    component()->set_text(text);
    return this;
}

UIFlatButtonBuilder *UIFlatButtonBuilder::LetCmdId(int cmd_id) {
    component()->set_cmd_id(cmd_id);
    return this;
}

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

////////////////////////////////////////////////////////////////////////////////
// class UIFlatInputBoxBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatInputBoxBuilder *UIFlatInputBoxBuilder::LetText(const char *text) {
    component()->set_text(text);
    return this;
}

UIFlatInputBoxBuilder *UIFlatInputBoxBuilder::LetMaxInput(int max_input) {
    component()->set_max_input(max_input);
    return this;
}

UIFlatInputBoxBuilder *UIFlatInputBoxBuilder::LetX(int x) {
    component()->mutable_rect()->x = x;
    return this;
}

UIFlatInputBoxBuilder *UIFlatInputBoxBuilder::LetY(int y) {
    component()->mutable_rect()->y = y;
    return this;
}

UIFlatInputBoxBuilder *UIFlatInputBoxBuilder::LetW(int w) {
    component()->mutable_rect()->w = w;
    return this;
}

UIFlatInputBoxBuilder *UIFlatInputBoxBuilder::LetH(int h) {
    component()->mutable_rect()->h = h;
    return this;
}

UIFlatInputBox *UIFlatInputBoxBuilder::EndInputBox() {
    auto result = component();
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UIFlatLabelBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatLabelBuilder *UIFlatLabelBuilder::LetText(const char *text) {
    component()->set_text(text);
    return this;
}

UIFlatLabelBuilder *UIFlatLabelBuilder::LetFontColor(int color) {
    component()->set_font_color(ToColor(color));
    return this;
}

UIFlatLabelBuilder *UIFlatLabelBuilder::LetBgColor(int color) {
    component()->set_bg_color(ToColor(color));
    return this;
}

UIFlatLabel *UIFlatLabelBuilder::EndLabel() {
    auto result = component();
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UIPicGridSelectorBuilder
////////////////////////////////////////////////////////////////////////////////

UIPicGridSelectorBuilder *UIPicGridSelectorBuilder::LetCmdId(int cmd_id) {
    component()->set_cmd_id(cmd_id);
    return this;
}

UIPicGridSelectorBuilder *UIPicGridSelectorBuilder::LetGridSizeW(int w) {
    component()->set_grid_size_w(w);
    return this;
}

UIPicGridSelectorBuilder *UIPicGridSelectorBuilder::LetGridSizeH(int h) {
    component()->set_grid_size_h(h);
    return this;
}

UIPicGridSelector *
UIPicGridSelectorBuilder::EndPicGridSelectorFromFile(const char *file) {
    if (!component()->LoadPicFromFile(file)) {
        LOG(ERROR) << "load file fail! " << file;
        return nullptr;
    }
    return EndPicGridSelector();
}

UIPicGridSelector *UIPicGridSelectorBuilder::EndPicGridSelector() {
    auto result = component();
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UIPicGridSelectorBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatCheckBoxBuilder *UIFlatCheckBoxBuilder::LetLabel(const char *text) {
    component()->set_label(text);
    return this;
}

UIFlatCheckBoxBuilder *UIFlatCheckBoxBuilder::LetValue(bool checked) {
    component()->set_checked(checked);
    return this;
}

UIFlatCheckBoxBuilder *UIFlatCheckBoxBuilder::LetX(int x) {
    component()->mutable_rect()->x = x;
    return this;
}

UIFlatCheckBoxBuilder *UIFlatCheckBoxBuilder::LetY(int y) {
    component()->mutable_rect()->y = y;
    return this;
}

UIFlatCheckBoxBuilder *UIFlatCheckBoxBuilder::LetW(int w) {
    component()->mutable_rect()->w = w;
    return this;
}

UIFlatCheckBoxBuilder *UIFlatCheckBoxBuilder::LetH(int h) {
    component()->mutable_rect()->h = h;
    return this;
}

UIFlatCheckBox *UIFlatCheckBoxBuilder::EndCheckBox() {
    auto result = component();
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UIFlatStatusBarBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatStatusBarGridBuilder *UIFlatStatusBarBuilder::BeginGrid(int w) {
    return new UIFlatStatusBarGridBuilder(this, component()->AddGrid("", w));
}

UIFlatStatusBarBuilder *UIFlatStatusBarBuilder::LetX(int x) {
    component()->mutable_rect()->x = x;
    return this;
}

UIFlatStatusBarBuilder *UIFlatStatusBarBuilder::LetY(int y) {
    component()->mutable_rect()->y = y;
    return this;
}

UIFlatStatusBarBuilder *UIFlatStatusBarBuilder::LetW(int w) {
    component()->mutable_rect()->w = w;
    return this;
}

UIFlatStatusBarBuilder *UIFlatStatusBarBuilder::LetH(int h) {
    component()->mutable_rect()->h = h;
    return this;
}

UIFlatStatusBar *UIFlatStatusBarBuilder::EndStatusBar() {
    auto result = component();
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UIFlatTextBoxBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatTextBoxBuilder *UIFlatTextBoxBuilder::LetX(int x) {
    component()->mutable_rect()->x = x;
    return this;
}

UIFlatTextBoxBuilder *UIFlatTextBoxBuilder::LetY(int y) {
    component()->mutable_rect()->y = y;
    return this;
}

UIFlatTextBoxBuilder *UIFlatTextBoxBuilder::LetW(int w) {
    component()->mutable_rect()->w = w;
    return this;
}

UIFlatTextBoxBuilder *UIFlatTextBoxBuilder::LetH(int h) {
    component()->mutable_rect()->h = h;
    return this;
}

UIFlatTextBox *UIFlatTextBoxBuilder::EndTextBox() {
    auto result = component();
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UIFlatStatusBarGridBuilder
////////////////////////////////////////////////////////////////////////////////

UIFlatStatusBarGridBuilder *
UIFlatStatusBarGridBuilder::LetText(const char *text) {
    grid_->set_text(text);
    return this;
}

UIFlatStatusBarGridBuilder *UIFlatStatusBarGridBuilder::LetW(int w) {
    grid_->set_w(w);
    return this;
}

UIFlatStatusBarGridBuilder *UIFlatStatusBarGridBuilder::LetFontColor(int color) {
    grid_->set_font_color(ToColor(color));
    return this;
}

UIFlatStatusBarGridBuilder *UIFlatStatusBarGridBuilder::LetBgColor(int color) {
    grid_->set_bg_color(ToColor(color));
    return this;
}

UIFlatStatusBarBuilder *UIFlatStatusBarGridBuilder::EndGrid() {
    auto reuslt = builder_;
    delete this;
    return reuslt;
}

////////////////////////////////////////////////////////////////////////////////
// class UIAnimatedAvatarViewBuilder
////////////////////////////////////////////////////////////////////////////////

UIAnimatedAvatarViewBuilder *UIAnimatedAvatarViewBuilder::LetViewW(int w) {
    component()->set_view_w(w);
    return this;
}

UIAnimatedAvatarViewBuilder *UIAnimatedAvatarViewBuilder::LetViewH(int h) {
    component()->set_view_h(h);
    return this;
}

UIAnimatedAvatarViewBuilder *
UIAnimatedAvatarViewBuilder::LetAnimatedSpeed(int speed) {
    component()->set_animated_speed(speed);
    return this;
}

UIAnimatedAvatarView *UIAnimatedAvatarViewBuilder::EndAnimatedAvatarView() {
    auto result = component();
    delete this;
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// class UITerrainViewBuilder
////////////////////////////////////////////////////////////////////////////////

UITerrainViewBuilder *UITerrainViewBuilder::LetHasRuler(bool value) {
    component()->set_has_ruler(value);
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetCmdId(int id) {
    component()->set_cmd_id(id);
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetTileW(int w) {
    component()->set_tile_w(w);
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetTileH(int h) {
    component()->set_tile_h(h);
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetScrollingSpeed(int speed) {
    component()->set_scrolling_speed(speed);
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetViewPortHTiles(int n) {
    component()->set_view_port_h_tiles(n);
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetViewPortVTiles(int n) {
    component()->set_view_port_v_tiles(n);
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetMaxHTiles(int n) {
    component()->set_max_h_tiles(n);
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetMaxVTiles(int n) {
    component()->set_max_v_tiles(n);
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetX(int x) {
    component()->mutable_rect()->x = x;
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetY(int y) {
    component()->mutable_rect()->y = y;
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetW(int w) {
    component()->mutable_rect()->w = w;
    return this;
}

UITerrainViewBuilder *UITerrainViewBuilder::LetH(int h) {
    component()->mutable_rect()->h = h;
    return this;
}

UITerrainView *UITerrainViewBuilder::EndTerrainView() {
    auto result = component();
    delete this;
    return result;
}

} // namespace utaha
