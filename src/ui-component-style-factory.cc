#include "ui-component-factory.h"
#include "ui-flat-button.h"
#include "ui-flat-menu.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-input-box.h"
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
    virtual UIFlatMenuGroup *CreateFlatMenuGroup(const std::string &name) override;
    virtual UIFlatInputBox *CreateFlatInputBox(const std::string &name) override;
    
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

static const char FLAT_MENU_GROUP_FONT[]           = "FlatMenuGroup.font";
static const char FLAT_MENU_GROUP_BG_COLOR[]       = "FlatMenuGroup.bg.color";
static const char FLAT_MENU_GROUP_HOT_COLOR[]      = "FlatMenuGroup.hot.color";
static const char FLAT_MENU_GROUP_FONT_COLOR[]     = "FlatMenuGroup.font.color";
static const char FLAT_MENU_GROUP_BORDER_COLOR[]   = "FlatMenuGroup.border.color";
static const char FLAT_MENU_GROUP_PADDING_H_SIZE[] = "FlatMenuGroup.padding.h.size";
static const char FLAT_MENU_GROUP_PADDING_V_SIZE[] = "FlatMenuGroup.padding.v.size";

static const char FLAT_BUTTON_FONT[]          = "FlatButton.font";
static const char FLAT_BUTTON_DEFAULT_SIZE[]  = "FlatButton.defaultSize";
static const char FLAT_BUTTON_HOT_COLOR[]     = "FlatButton.hot.color";
static const char FLAT_BUTTON_FONT_COLOR[]    = "FlatButton.font.color";
static const char FLAT_BUTTON_PRESSED_COLOR[] = "FlatButton.pressed.color";
static const char FLAT_BUTTON_NORMAL_COLOR[]  = "FlatButton.bg.color";

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
        LOG(ERROR) << "can not find font: " << FLAT_BUTTON_FONT;
        return nullptr;
    }
    std::unique_ptr<UIFlatButton> component(new UIFlatButton(font));
    component->set_name(name);
    component->set_id(NextId());
    if (!style_->FindColor(FLAT_BUTTON_PRESSED_COLOR, component->mutable_pressed_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_BUTTON_PRESSED_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_BUTTON_NORMAL_COLOR, component->mutable_normal_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_BUTTON_NORMAL_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_BUTTON_HOT_COLOR, component->mutable_hot_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_BUTTON_HOT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_BUTTON_FONT_COLOR, component->mutable_font_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_BUTTON_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindSize(FLAT_BUTTON_DEFAULT_SIZE, &component->mutable_rect()->w,
                          &component->mutable_rect()->h)) {
        LOG(ERROR) << "can not find size: " << FLAT_BUTTON_DEFAULT_SIZE;
        return nullptr;
    }
    return component.release();
}

/*virtual*/ UIFlatMenu *
UIComponentStyleFactory::CreateFlatMenu(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(FLAT_MENU_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "can not find font: " << FLAT_MENU_FONT;
        return nullptr;
    }

    std::unique_ptr<UIFlatMenu> component(new UIFlatMenu(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(FLAT_MENU_BG_COLOR, component->mutable_bg_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_MENU_BG_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_HOT_COLOR, component->mutable_hot_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_MENU_HOT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_FONT_COLOR, component->mutable_font_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_MENU_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_BORDER_COLOR, component->mutable_border_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_MENU_BORDER_COLOR;
        return nullptr;
    }
    component->set_padding_size(style_->FindInt(FLAT_MENU_PADDING_SIZE, &ok));
    if (!ok) {
        LOG(ERROR) << "can not find int value: " << FLAT_MENU_PADDING_SIZE;
        return nullptr;
    }
    return component.release();
}

/*virtual*/ UIFlatMenuGroup *
UIComponentStyleFactory::CreateFlatMenuGroup(const std::string &name) {
    bool ok = true;
    TTF_Font *font = style_->FindFont(FLAT_MENU_GROUP_FONT, &ok);
    if (!ok) {
        LOG(ERROR) << "can not find font: " << FLAT_MENU_GROUP_FONT;
        return nullptr;
    }

    std::unique_ptr<UIFlatMenuGroup> component(new UIFlatMenuGroup(font));
    component->set_name(name);
    component->set_id(NextId());

    if (!style_->FindColor(FLAT_MENU_GROUP_BG_COLOR, component->mutable_bg_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_MENU_GROUP_BG_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_GROUP_HOT_COLOR, component->mutable_hot_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_MENU_GROUP_HOT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_GROUP_FONT_COLOR, component->mutable_font_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_MENU_GROUP_FONT_COLOR;
        return nullptr;
    }
    if (!style_->FindColor(FLAT_MENU_GROUP_BORDER_COLOR, component->mutable_border_color())) {
        LOG(ERROR) << "can not find color: " << FLAT_MENU_GROUP_BORDER_COLOR;
        return nullptr;
    }
    component->set_h_padding_size(style_->FindInt(FLAT_MENU_GROUP_PADDING_H_SIZE,
                                                  &ok));
    if (!ok) {
        LOG(ERROR) << "can not find int value: " << FLAT_MENU_GROUP_PADDING_H_SIZE;
        return nullptr;
    }
    component->set_v_padding_size(style_->FindInt(FLAT_MENU_GROUP_PADDING_V_SIZE,
                                                  &ok));
    if (!ok) {
        LOG(ERROR) << "can not find int value: " << FLAT_MENU_GROUP_PADDING_V_SIZE;
        return nullptr;
    }

    return component.release();
}

/*virtual*/ UIFlatInputBox *
UIComponentStyleFactory::CreateFlatInputBox(const std::string &name) {
    return nullptr;
}


} // namespace utaha
