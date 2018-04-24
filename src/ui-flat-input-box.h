#ifndef UTAHA_UI_FLAT_INPUT_BOX_H_
#define UTAHA_UI_FLAT_INPUT_BOX_H_

#include "ui-component.h"
#include <string>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class SampleTextLabel;

class UIFlatInputBox : public UIComponent {
public:
    UIFlatInputBox(TTF_Font *font);
    virtual ~UIFlatInputBox();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;

    DEF_VAL_PROP_RMW(std::string, text);
    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RMW(SDL_Color, bg_color);
    DEF_VAL_PROP_RMW(SDL_Color, text_color);
    DEF_VAL_PROP_RW(int, max_input);
    DEF_VAL_PROP_RW(int, padding_size);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatInputBox);
private:
    bool CreateOrSetText(SDL_Renderer *renderer);

    std::string text_;
    SDL_Color border_color_;
    SDL_Color bg_color_;
    SDL_Color text_color_;
    int max_input_ = 1024;
    int padding_size_ = 2;
    bool is_changed_ = false;
    TTF_Font *font_;
    std::unique_ptr<SampleTextLabel> label_;
}; // class UIFlatInputBox

} // namespace utaha

#endif // UTAHA_UI_FLAT_INPUT_BOX_H_
