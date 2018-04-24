#ifndef UI_FLAT_BUTTON_H_
#define UI_FLAT_BUTTON_H_

#include "ui-component.h"
#include "base.h"
#include <memory>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class SampleTextLabel;

class UIFlatButton : public UIComponent {
public:
    UIFlatButton();
    virtual ~UIFlatButton();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;

    DEF_VAL_PROP_RMW(SDL_Color, normal_color);
    DEF_VAL_PROP_RMW(SDL_Color, pressed_color);

    bool SetNormalText(const char *text, SDL_Color fg, TTF_Font *font,
                       SDL_Renderer *renderer);
    bool SetPressedText(const char *text, SDL_Color fg, TTF_Font *font,
                        SDL_Renderer *renderer);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatButton);
private:
    SampleTextLabel *
    CreateTextLabel(const char *text, SDL_Color fg, TTF_Font *font,
                    SDL_Renderer *renderer);

    bool is_pressed_ = false;
    SDL_Color pressed_color_ = {0, 0, 0, 0};
    SDL_Color normal_color_  = {0, 0, 0, 0};
    std::unique_ptr<SampleTextLabel> pressed_;
    std::unique_ptr<SampleTextLabel> normal_;
}; // class UIFlatButton

} // namespace utaha

#endif // UI_FLAT_BUTTON_H_
