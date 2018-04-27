#ifndef UTAHA_UI_FLAG_MENU_H_
#define UTAHA_UI_FLAG_MENU_H_

#include "ui-component.h"
#include <vector>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

struct UIFlatMenuItem {
    enum Kind {
        MENU_ITEM,
        MENU_DIV,
    };
    enum State {
        STATE_NORMAL,
        STATE_HOT,
    };

    Kind        kind;
    State       state;
    bool        is_selected;
    std::string name;
    int         cmd_id;
    int         y;
    int         h;
    void       *param;
};

class UIFlatMenu : public UIComponent {
public:
    typedef UIFlatMenuItem Item;

    UIFlatMenu(TTF_Font *font);
    virtual ~UIFlatMenu();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;

    bool Popup(int x, int y, SDL_Renderer *renderer);

    UIFlatMenu *AddItem(const char *name, int cmd_id, void *param);
    UIFlatMenu *AddDiv();

    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RMW(SDL_Color, bg_color);
    DEF_VAL_PROP_RMW(SDL_Color, hot_color);
    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RW(int, padding_size);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatMenu);
private:
    SDL_Texture *CreateTexture(int x, int y, SDL_Renderer *renderer);

    TTF_Font *font_;
    SDL_Color border_color_ = {0, 0, 0, 0};
    SDL_Color bg_color_ = {0, 0, 0, 0};
    SDL_Color hot_color_ = {0, 0, 0, 0};
    SDL_Color font_color_ = {0, 0, 0, 0};
    std::vector<Item> items_;
    SDL_Texture *texture_ = nullptr;
    int padding_size_ = 2;
    bool is_changed_ = false;
}; // class UIFlatMenu

} // namespace utaha

#endif // UTAHA_UI_FLAG_MENU_H_
