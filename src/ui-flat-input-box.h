#ifndef UTAHA_UI_FLAT_INPUT_BOX_H_
#define UTAHA_UI_FLAT_INPUT_BOX_H_

#include "ui-component.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class UIFlatInputBox : public UIComponent {
public:
    UIFlatInputBox(TTF_Font *font);
    virtual ~UIFlatInputBox();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;

    DEF_VAL_PROP_RMW(std::string, text);
    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RMW(SDL_Color, bg_color);
    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RW(int, max_input);
    DEF_VAL_PROP_RW(int, padding_size);

    void changed() { is_changed_ = true; }

    int GetInt() const { return atoi(text_.c_str()); }
    inline void SetInt(int value);

    void SetText(const std::string &text) {
        text_.assign(text);
        is_changed_= true;
    }

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatInputBox);
private:
    bool CreateOrSetText(SDL_Renderer *renderer);

    TTF_Font *font_;
    std::string text_;
    SDL_Color border_color_ = {0, 0, 0, 0};
    SDL_Color bg_color_ = {0, 0, 0, 0};
    SDL_Color font_color_ = {0, 0, 0, 0};
    int max_input_ = 1024;
    int padding_size_ = 2;
    bool is_changed_ = false;
    SDL_Texture *texture_ = nullptr;
    int text_w_ = 0;
    int text_h_ = 0;
}; // class UIFlatInputBox

inline void UIFlatInputBox::SetInt(int value) {
    char buf[128];
    snprintf(buf, arraysize(buf), "%d", value);
    text_.assign(buf);
    is_changed_ = true;
}

} // namespace utaha

#endif // UTAHA_UI_FLAT_INPUT_BOX_H_
