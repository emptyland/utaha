#include "ui-component-factory.h"
#include "ui-flat-button.h"
#include "ui-flat-menu.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-input-box.h"
#include "ui-flat-label.h"
#include "ui-flat-check-box.h"
#include "ui-flat-status-bar.h"
#include "ui-flat-pic-view.h"
#include "ui-flat-text-box.h"
#include "ui-terrain-view.h"
#include "ui-pic-grid-selector.h"
#include "ui-animated-avatar-view.h"
#include "ui-style-collection.h"
#include "glog/logging.h"
#include <memory>

namespace utaha {

class UIComponentStyleFactory : public UIComponentFactory {
public:
    UIComponentStyleFactory(UIStyleCollection *style);
    virtual ~UIComponentStyleFactory();

    virtual UIFlatButton *CreateFlatButton(const std::string &name) override;

    virtual UIFlatMenu *CreateFlatMenu(const std::string &name) override;

    virtual UIFlatMenuGroup *
    CreateFlatMenuGroup(const std::string &name) override;

    virtual UIFlatInputBox *
    CreateFlatInputBox(const std::string &name) override;

    virtual UIFlatLabel *CreateFlatLabel(const std::string &name) override;

    virtual UIFlatCheckBox *
    CreateFlatCheckBox(const std::string &name) override;

    virtual UIFlatStatusBar *
    CreateFlatStatusBar(const std::string &name) override;

    virtual UIFlatPicView *CreateFlatPicView(const std::string &name) override;

    virtual UIFlatTextBox *CreateFlatTextBox(const std::string &name) override;

    virtual UIPicGridSelector *
    CreatePicGridSelector(const std::string &name) override;

    virtual UIAnimatedAvatarView *
    CreateAnimatedAvatarView(const std::string &name) override;

    virtual UITerrainView *CreateTerrainView(const std::string &name) override;
    
    DISALLOW_IMPLICIT_CONSTRUCTORS(UIComponentStyleFactory);
private:
    int64_t NextId() { return unique_id_ += 100; }

    UIStyleCollection *style_;
    int64_t unique_id_ = 100;
}; // class UIComponentStyleFactory

static const char FLAT_MENU_FONT[]         = "FlatMenu.font";
static const char FLAT_MENU_BG_COLOR[]     = "FlatMenu.bg.color";
static const char FLAT_MENU_HOT_COLOR[]    = "FlatMenu.hot.color";
static const char FLAT_MENU_FONT_COLOR[]   = "FlatMenu.font.color";
static const char FLAT_MENU_BORDER_COLOR[] = "FlatMenu.border.color";
static const char FLAT_MENU_PADDING_SIZE[] = "FlatMenu.padding.size";

static const char FLAT_MENU_GROUP_FONT[]         = "FlatMenuGroup.font";
static const char FLAT_MENU_GROUP_BG_COLOR[]     = "FlatMenuGroup.bg.color";
static const char FLAT_MENU_GROUP_HOT_COLOR[]    = "FlatMenuGroup.hot.color";
static const char FLAT_MENU_GROUP_FONT_COLOR[]   = "FlatMenuGroup.font.color";
static const char FLAT_MENU_GROUP_BORDER_COLOR[] = "FlatMenuGroup.border.color";
static const char FLAT_MENU_GROUP_PADDING_H_SIZE[]
    = "FlatMenuGroup.padding.h.size";
static const char FLAT_MENU_GROUP_PADDING_V_SIZE[]
    = "FlatMenuGroup.padding.v.size";

static const char FLAT_BUTTON_FONT[]          = "FlatButton.font";
static const char FLAT_BUTTON_DEFAULT_SIZE[]  = "FlatButton.defaultSize";
static const char FLAT_BUTTON_HOT_COLOR[]     = "FlatButton.hot.color";
static const char FLAT_BUTTON_FONT_COLOR[]    = "FlatButton.font.color";
static const char FLAT_BUTTON_PRESSED_COLOR[] = "FlatButton.pressed.color";
static const char FLAT_BUTTON_NORMAL_COLOR[]  = "FlatButton.bg.color";

static const char FLAT_INPUT_BOX_FONT[]          = "FlatInputBox.font";
static const char FLAT_INPUT_BOX_DEFAULT_SIZE[]  = "FlatInputBox.defaultSize";
static const char FLAT_INPUT_BOX_DEFAULT_MAX_INPUT[]
    = "FlatInputBox.defaultMaxInput";
static const char FLAT_INPUT_BOX_FONT_COLOR[]    = "FlatInputBox.font.color";
static const char FLAT_INPUT_BOX_BORDER_COLOR[]  = "FlatInputBox.border.color";
static const char FLAT_INPUT_BOX_BG_COLOR[]      = "FlatInputBox.bg.color";
static const char FLAT_INPUT_BOX_PADDING_SIZE[]  = "FlatInputBox.padding.size";

static const char FLAT_LABEL_FONT[]           = "FlatLabel.font";
static const char FLAT_LABEL_BG_COLOR[]       = "FlatLabel.bg.color";
static const char FLAT_LABEL_FONT_COLOR[]     = "FlatLabel.font.color";
static const char FLAT_LABEL_BORDER_COLOR[]   = "FlatLabel.border.color";
static const char FLAT_LABEL_PADDING_H_SIZE[] = "FlatLabel.padding.h.size";
static const char FLAT_LABEL_PADDING_V_SIZE[] = "FlatLabel.padding.v.size";

static const char FLAT_CHECK_BOX_FONT[]       = "FlatCheckBox.font";
static const char FLAT_CHECK_BOX_FONT_COLOR[] = "FlatCheckBox.font.color";
static const char FLAT_CHECK_BOX_BOX_COLOR[]  = "FlatCheckBox.box.color";
static const char FLAT_CHECK_BOX_PADDING_H_SIZE[]
    = "FlatCheckBox.padding.h.size";
static const char FLAT_CHECK_BOX_PADDING_V_SIZE[]
    = "FlatCheckBox.padding.v.size";

static const char FLAT_STATUS_BAR_FONT[]         = "FlatStatusBar.font";
static const char FLAT_STATUS_BAR_FONT_COLOR[]   = "FlatStatusBar.font.color";
static const char FLAT_STATUS_BAR_BG_COLOR[]     = "FlatStatusBar.bg.color";
static const char FLAT_STATUS_BAR_BORDER_COLOR[] = "FlatStatusBar.border.color";
static const char FLAT_STATUS_BAR_PADDING_SIZE[] = "FlatStatusBar.padding.size";

const static char FLAT_TEXT_BOX_FONT[] = "FlatTextBox.font";
const static char FLAT_TEXT_BOX_FONT_COLOR[] = "FlatTextBox.font.color";
const static char FLAT_TEXT_BOX_PADDING_SIZE[] = "FlatTextBox.padding.size";
const static char FLAT_TEXT_BOX_BORDER_COLOR[] = "FlatTextBox.border.color";

static const char FLAT_PIC_VIEW_BORDER_COLOR[]   = "FlatPicView.border.color";
static const char FLAT_PIC_VIEW_PADDING_SIZE[]   = "FlatPicView.padding.size";

static const char PIC_GRID_SELECTOR_GRID_COLOR[] = "PicGridSelector.grid.color";
static const char PIC_GRID_SELECTOR_SELECTED_COLOR[]
    = "PicGridSelector.selected.color";

static const char ANIMATED_AVATAR_VIEW_FONT[] = "AnimatedAvatarView.font";
static const char ANIMATED_AVATAR_VIEW_FONT_COLOR[]
    = "AnimatedAvatarView.font.color";
static const char ANIMATED_AVATAR_VIEW_BORDER_COLOR[]
    = "AnimatedAvatarView.border.color";
static const char ANIMATED_AVATAR_VIEW_PADDING_SIZE[]
    = "AnimatedAvatarView.padding.size";

static const char TERRAIN_VIEW_FONT[] = "TerrainView.font";
static const char TERRAIN_VIEW_FONT_COLOR[] = "TerrainView.font.color";
static const char TERRAIN_VIEW_BORDER_COLOR[] = "TerrainView.border.color";
static const char TERRAIN_VIEW_GRID_COLOR[] = "TerrainView.grid.color";
static const char TERRAIN_VIEW_PADDING_SIZE[] = "TerrainView.padding.size";

UIComponentFactory *CreateUIComponentStyleFactory(UIStyleCollection *style) {
    return new UIComponentStyleFactory(style);
}

UIComponentStyleFactory::UIComponentStyleFactory(UIStyleCollection *style)
    : style_(DCHECK_NOTNULL(style)) {
}

/*virtual*/ UIComponentStyleFactory::~UIComponentStyleFactory() {
}

/*virtual*/ UIFlatButton *
UIComponentStyleFactory::CreateFlatButton(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(FLAT_BUTTON_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "Can not find font: " << FLAT_BUTTON_FONT;
        return nullptr;
    }
    std::unique_ptr<UIFlatButton> component(new UIFlatButton(font));
    component->set_name(name);
    component->set_id(NextId());
    if (!style_->FindColor(FLAT_BUTTON_PRESSED_COLOR,
                           component->mutable_pressed_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_BUTTON_PRESSED_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_BUTTON_NORMAL_COLOR,
                           component->mutable_normal_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_BUTTON_NORMAL_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_BUTTON_HOT_COLOR,
                           component->mutable_hot_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_BUTTON_HOT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_BUTTON_FONT_COLOR,
                           component->mutable_font_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_BUTTON_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindSize(FLAT_BUTTON_DEFAULT_SIZE,
                          &component->mutable_rect()->w,
                          &component->mutable_rect()->h)) {
        LOG(ERROR) << "Can not find size: " << FLAT_BUTTON_DEFAULT_SIZE;
        return nullptr;
    }
    return component.release();
}

/*virtual*/ UIFlatMenu *
UIComponentStyleFactory::CreateFlatMenu(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(FLAT_MENU_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "Can not find font: " << FLAT_MENU_FONT;
        return nullptr;
    }

    std::unique_ptr<UIFlatMenu> component(new UIFlatMenu(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(FLAT_MENU_BG_COLOR, component->mutable_bg_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_MENU_BG_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_HOT_COLOR,
                           component->mutable_hot_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_MENU_HOT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_FONT_COLOR,
                           component->mutable_font_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_MENU_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_BORDER_COLOR,
                           component->mutable_border_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_MENU_BORDER_COLOR;
        return nullptr;
    }
    component->set_padding_size(style_->FindInt(FLAT_MENU_PADDING_SIZE, &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: " << FLAT_MENU_PADDING_SIZE;
        return nullptr;
    }
    return component.release();
}

/*virtual*/ UIFlatMenuGroup *
UIComponentStyleFactory::CreateFlatMenuGroup(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(FLAT_MENU_GROUP_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "Can not find font: " << FLAT_MENU_GROUP_FONT;
        return nullptr;
    }

    std::unique_ptr<UIFlatMenuGroup> component(new UIFlatMenuGroup(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(FLAT_MENU_GROUP_BG_COLOR,
                           component->mutable_bg_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_MENU_GROUP_BG_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_GROUP_HOT_COLOR,
                           component->mutable_hot_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_MENU_GROUP_HOT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_GROUP_FONT_COLOR,
                           component->mutable_font_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_MENU_GROUP_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_GROUP_BORDER_COLOR,
                           component->mutable_border_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_MENU_GROUP_BORDER_COLOR;
        return nullptr;
    }
    component->set_h_padding_size(style_->FindInt(FLAT_MENU_GROUP_PADDING_H_SIZE,
                                                  &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: "
                   << FLAT_MENU_GROUP_PADDING_H_SIZE;
        return nullptr;
    }
    component->set_v_padding_size(style_->FindInt(FLAT_MENU_GROUP_PADDING_V_SIZE,
                                                  &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: "
                   << FLAT_MENU_GROUP_PADDING_V_SIZE;
        return nullptr;
    }

    return component.release();
}

/*virtual*/ UIFlatInputBox *
UIComponentStyleFactory::CreateFlatInputBox(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(FLAT_INPUT_BOX_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "Can not find font: " << FLAT_INPUT_BOX_FONT;
        return nullptr;
    }

    std::unique_ptr<UIFlatInputBox> component(new UIFlatInputBox(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindSize(FLAT_INPUT_BOX_DEFAULT_SIZE,
                          &component->mutable_rect()->w,
                          &component->mutable_rect()->h)) {
        LOG(ERROR) << "Can not find size: " << FLAT_INPUT_BOX_DEFAULT_SIZE;
        return nullptr;
    }
    component->set_max_input(style_->FindInt(FLAT_INPUT_BOX_DEFAULT_MAX_INPUT,
                                             &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: "
                   << FLAT_INPUT_BOX_DEFAULT_MAX_INPUT;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_INPUT_BOX_BG_COLOR,
                           component->mutable_bg_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_INPUT_BOX_BG_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_INPUT_BOX_FONT_COLOR,
                           component->mutable_font_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_INPUT_BOX_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_INPUT_BOX_BORDER_COLOR,
                           component->mutable_border_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_INPUT_BOX_BORDER_COLOR;
        return nullptr;
    }
    component->set_padding_size(style_->FindInt(FLAT_INPUT_BOX_PADDING_SIZE,
                                                &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: " << FLAT_INPUT_BOX_PADDING_SIZE;
        return nullptr;
    }

    return component.release();
}

UIFlatLabel *UIComponentStyleFactory::CreateFlatLabel(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(FLAT_LABEL_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "Can not find font: " << FLAT_LABEL_FONT;
        return nullptr;
    }

    std::unique_ptr<UIFlatLabel> component(new UIFlatLabel(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(FLAT_LABEL_BG_COLOR,
                           component->mutable_bg_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_LABEL_BG_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_LABEL_FONT_COLOR,
                           component->mutable_font_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_LABEL_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_LABEL_BORDER_COLOR,
                           component->mutable_border_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_LABEL_BORDER_COLOR;
        return nullptr;
    }
    component->set_h_padding_size(style_->FindInt(FLAT_LABEL_PADDING_H_SIZE,
                                                  &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: " << FLAT_LABEL_PADDING_H_SIZE;
        return nullptr;
    }
    component->set_v_padding_size(style_->FindInt(FLAT_LABEL_PADDING_V_SIZE,
                                                  &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: " << FLAT_LABEL_PADDING_V_SIZE;
        return nullptr;
    }
    return component.release();
}

/*virtual*/ UIFlatCheckBox *
UIComponentStyleFactory::CreateFlatCheckBox(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(FLAT_CHECK_BOX_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "Can not find font: " << FLAT_CHECK_BOX_FONT;
        return nullptr;
    }

    std::unique_ptr<UIFlatCheckBox> component(new UIFlatCheckBox(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(FLAT_CHECK_BOX_FONT_COLOR,
                           component->mutable_font_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_CHECK_BOX_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_CHECK_BOX_BOX_COLOR,
                           component->mutable_box_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_CHECK_BOX_BOX_COLOR;
        return nullptr;
    }

    component->set_h_padding_size(style_->FindInt(FLAT_CHECK_BOX_PADDING_H_SIZE,
                                                  &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: "
                   << FLAT_CHECK_BOX_PADDING_H_SIZE;
        return nullptr;
    }
    component->set_v_padding_size(style_->FindInt(FLAT_CHECK_BOX_PADDING_V_SIZE,
                                                  &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: "
                   << FLAT_CHECK_BOX_PADDING_V_SIZE;
        return nullptr;
    }

    return component.release();
}

/*virtual*/ UIFlatStatusBar *
UIComponentStyleFactory::CreateFlatStatusBar(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(FLAT_STATUS_BAR_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "Can not find font: " << FLAT_STATUS_BAR_FONT;
        return nullptr;
    }

    std::unique_ptr<UIFlatStatusBar> component(new UIFlatStatusBar(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(FLAT_STATUS_BAR_FONT_COLOR,
                           component->mutable_font_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_STATUS_BAR_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_STATUS_BAR_BG_COLOR,
                           component->mutable_bg_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_STATUS_BAR_BG_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_STATUS_BAR_BORDER_COLOR,
                           component->mutable_border_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_STATUS_BAR_BORDER_COLOR;
        return nullptr;
    }
    component->set_padding_size(style_->FindInt(FLAT_STATUS_BAR_PADDING_SIZE,
                                                &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: "
                   << FLAT_STATUS_BAR_PADDING_SIZE;
        return nullptr;
    }
    return component.release();
}

/*virtual*/ UIFlatPicView *
UIComponentStyleFactory::CreateFlatPicView(const std::string &name) {
    std::unique_ptr<UIFlatPicView> component(new UIFlatPicView());
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(FLAT_PIC_VIEW_BORDER_COLOR,
                           component->mutable_border_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_PIC_VIEW_BORDER_COLOR;
        return nullptr;
    }
    bool ok = true;
    component->set_padding_size(style_->FindInt(FLAT_PIC_VIEW_PADDING_SIZE,
                                                &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: " << FLAT_PIC_VIEW_PADDING_SIZE;
        return nullptr;
    }
    return component.release();
}

/*virtual*/ UIFlatTextBox *
UIComponentStyleFactory::CreateFlatTextBox(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(FLAT_TEXT_BOX_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "Can not find font: " << FLAT_TEXT_BOX_FONT;
        return nullptr;
    }

    std::unique_ptr<UIFlatTextBox> component(new UIFlatTextBox(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(FLAT_TEXT_BOX_FONT_COLOR,
                           component->mutable_font_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_TEXT_BOX_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_TEXT_BOX_BORDER_COLOR,
                           component->mutable_border_color())) {
        LOG(ERROR) << "Can not find color: " << FLAT_TEXT_BOX_BORDER_COLOR;
        return nullptr;
    }
    component->set_padding_size(style_->FindInt(FLAT_TEXT_BOX_PADDING_SIZE,
                                                &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: " << FLAT_TEXT_BOX_PADDING_SIZE;
        return nullptr;
    }
    return component.release();
}

/*virtual*/ UIPicGridSelector *
UIComponentStyleFactory::CreatePicGridSelector(const std::string &name) {
    std::unique_ptr<UIPicGridSelector> component(new UIPicGridSelector());
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(PIC_GRID_SELECTOR_GRID_COLOR,
                           component->mutable_grid_color())) {
        LOG(ERROR) << "Can not find color: " << PIC_GRID_SELECTOR_GRID_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(PIC_GRID_SELECTOR_SELECTED_COLOR,
                           component->mutable_selected_color())) {
        LOG(ERROR) << "Can not find color: "
                   << PIC_GRID_SELECTOR_SELECTED_COLOR;
        return nullptr;
    }
    return component.release();
}

/*virtual*/ UIAnimatedAvatarView *
UIComponentStyleFactory::CreateAnimatedAvatarView(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(ANIMATED_AVATAR_VIEW_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "Can not find font: " << ANIMATED_AVATAR_VIEW_FONT;
        return nullptr;
    }

    std::unique_ptr<UIAnimatedAvatarView> component(
        new UIAnimatedAvatarView(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(ANIMATED_AVATAR_VIEW_FONT_COLOR,
                           component->mutable_font_color())) {
        LOG(ERROR) << "Can not find color: " << ANIMATED_AVATAR_VIEW_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(ANIMATED_AVATAR_VIEW_BORDER_COLOR,
                           component->mutable_border_color())) {
        LOG(ERROR) << "Can not find color: "
                   << ANIMATED_AVATAR_VIEW_BORDER_COLOR;
        return nullptr;
    }
    component->set_padding_size(style_->FindInt(
            ANIMATED_AVATAR_VIEW_PADDING_SIZE, &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: "
                   << ANIMATED_AVATAR_VIEW_PADDING_SIZE;
        return nullptr;
    }

    return component.release();
}

/*virtual*/ UITerrainView *
UIComponentStyleFactory::CreateTerrainView(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(TERRAIN_VIEW_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "Can not find font: " << TERRAIN_VIEW_FONT;
        return nullptr;
    }

    std::unique_ptr<UITerrainView>
        component(new UITerrainView(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(TERRAIN_VIEW_FONT_COLOR,
                           component->mutable_font_color())) {
        LOG(ERROR) << "Can not find color: " << TERRAIN_VIEW_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(TERRAIN_VIEW_BORDER_COLOR,
                           component->mutable_border_color())) {
        LOG(ERROR) << "Can not find color: " << TERRAIN_VIEW_BORDER_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(TERRAIN_VIEW_GRID_COLOR,
                           component->mutable_grid_color())) {
        LOG(ERROR) << "Can not find color: " << TERRAIN_VIEW_GRID_COLOR;
        return nullptr;
    }
    component->set_padding_size(style_->FindInt(TERRAIN_VIEW_PADDING_SIZE,
                                                &ok));
    if (!ok) {
        LOG(ERROR) << "Can not find int value: " << TERRAIN_VIEW_PADDING_SIZE;
        return nullptr;
    }
    return component.release();
}

} // namespace utaha
