#ifndef UTAHA_UI_FLAT_TEXT_BOX_H_
#define UTAHA_UI_FLAT_TEXT_BOX_H_

#include "ui-component.h"
#include <string>
#include <vector>
#include <tuple>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class UIFlatTextBox : public UIComponent {
public:
    UIFlatTextBox(TTF_Font *font);
    virtual ~UIFlatTextBox();

    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RW(int, padding_size);

    virtual int OnRender(SDL_Renderer *renderer) override;

    void SetLinesSize(int size);
    void SetLine(int i, const std::string &text);
    void SetFormatLine(int i, const char *fmt, ...);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatTextBox);
private:
    TTF_Font *font_;
    SDL_Color font_color_ = {0, 0, 0, 0};
    SDL_Color border_color_ = {0, 0, 0, 0};
    int padding_size_ = 4;
    std::vector<std::tuple<std::string, SDL_Texture *>> lines_;
}; // class UIFlatTextBox

} // namespace utaha

#endif // UTAHA_UI_FLAT_TEXT_BOX_H_
