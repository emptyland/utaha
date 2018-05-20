#ifndef UTAHA_UI_FLAT_LABEL_H_
#define UTAHA_UI_FLAT_LABEL_H_

#include "ui-component.h"
#include <string>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class UIFlatLabel : public UIComponent {
public:
    UIFlatLabel(TTF_Font *font);
    virtual ~UIFlatLabel();

    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;
    virtual void UpdateRect() override;

    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RMW(SDL_Color, bg_color);
    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RW(int, h_padding_size);
    DEF_VAL_PROP_RW(int, v_padding_size);
    DEF_VAL_PROP_RMW(std::string, text);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatLabel);
private:
    TTF_Font *font_;
    SDL_Color font_color_ = {0, 0, 0, 0};
    SDL_Color bg_color_ = {0, 0, 0, 0};
    SDL_Color border_color_ = {0, 0, 0, 0};
    int h_padding_size_ = 2;
    int v_padding_size_ = 10;
    std::string text_;
    SDL_Texture *texture_ = nullptr;
}; // class UIFlatLabel

} // namespace utaha

#endif // UTAHA_UI_FLAT_LABEL_H_
