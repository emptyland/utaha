#ifndef UTAHA_UI_FLAT_CHECK_BOX_H_
#define UTAHA_UI_FLAT_CHECK_BOX_H_

#include "ui-component.h"
#include <string>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class UIFlatCheckBox : public UIComponent {
public:
    UIFlatCheckBox(TTF_Font *font);
    virtual ~UIFlatCheckBox();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;
    virtual void UpdateRect() override;

    DEF_VAL_PROP_RMW(SDL_Color, box_color);
    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RMW(std::string, label);
    DEF_VAL_PROP_RW(bool, checked);
    DEF_VAL_PROP_RW(int, h_padding_size);
    DEF_VAL_PROP_RW(int, v_padding_size);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatCheckBox);
private:
    SDL_Surface *CreateSurface();

    TTF_Font *font_;
    SDL_Color box_color_ = {0, 0, 0, 0};
    SDL_Color font_color_ = {0, 0, 0, 0};
    int h_padding_size_ = 2;
    int v_padding_size_ = 10;
    int box_size_ = 0;
    std::string label_;
    bool checked_ = false;
    SDL_Texture *texture_ = nullptr;
    int label_w_ = 0;
    int label_h_ = 0;
}; // class UIFlatCheckBox

} // namespace utaha

#endif // UTAHA_UI_FLAT_CHECK_BOX_H_
