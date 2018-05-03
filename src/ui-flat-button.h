#ifndef UI_FLAT_BUTTON_H_
#define UI_FLAT_BUTTON_H_

#include "ui-component.h"
#include "base.h"
#include <memory>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class UIFlatButton : public UIComponent {
public:
    enum State: int {
        STATE_NORMAL,
        STATE_PRESSED,
        STATE_HOT,
    };

    UIFlatButton(TTF_Font *font);
    virtual ~UIFlatButton();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;

    DEF_VAL_PROP_RMW(SDL_Color, normal_color);
    DEF_VAL_PROP_RMW(SDL_Color, pressed_color);
    DEF_VAL_PROP_RMW(SDL_Color, hot_color);
    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RMW(std::string, text);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatButton);
private:

    State state_ = STATE_NORMAL;
    SDL_Color pressed_color_ = {0, 0, 0, 0};
    SDL_Color normal_color_  = {0, 0, 0, 0};
    SDL_Color hot_color_     = {0, 0, 0, 0};
    SDL_Color font_color_    = {0, 0, 0, 0};
    std::string text_;
    SDL_Texture *texture_ = nullptr;
    int text_w_ = 0;
    int text_h_ = 0;
    TTF_Font *font_;
}; // class UIFlatButton

} // namespace utaha

#endif // UI_FLAT_BUTTON_H_
