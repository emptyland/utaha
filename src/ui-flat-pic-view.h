#ifndef UTAHA_UI_FLAT_PIC_VIEW_H_
#define UTAHA_UI_FLAT_PIC_VIEW_H_

#include "ui-component.h"

namespace utaha {

class UIFlatPicView : public UIComponent {
public:
    UIFlatPicView();
    virtual ~UIFlatPicView();

    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;
    virtual void UpdateRect() override;

    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RW(int, padding_size);
    DEF_PTR_GETTER(SDL_Surface, pic);

    void SetPic(SDL_Surface *pic, bool ownership);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatPicView);

private:
    SDL_Color border_color_ = {0, 0, 0, 0};
    int padding_size_ = 0;
    SDL_Surface *pic_ = nullptr;
    bool ownership_ = false;
    SDL_Texture *texture_ = nullptr;
}; // class UIFlatPicView

} // namespace utaha

#endif // UTAHA_UI_FLAT_PIC_VIEW_H_
