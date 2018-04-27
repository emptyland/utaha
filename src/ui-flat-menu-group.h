#ifndef UTAHA_UI_FLAT_MENU_GROUP_H_
#define UTAHA_UI_FLAT_MENU_GROUP_H_

#include "ui-component.h"
#include <vector>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class UIFlatMenu;

struct UIFlatMenuGroupColumn {
    bool        is_hot;
    std::string name;
    int         cmd_id;
    int         x;
    int         w;
    UIFlatMenu *menu;
};

class UIFlatMenuGroup : public UIComponent {
public:
    typedef UIFlatMenuGroupColumn Column;

    UIFlatMenuGroup(TTF_Font *font);
    virtual ~UIFlatMenuGroup();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;

    void AddColumn(const char *name, int cmd_id, UIFlatMenu *menu);

    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RMW(SDL_Color, bg_color);
    DEF_VAL_PROP_RMW(SDL_Color, hot_color);
    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RW(int, h_padding_size);
    DEF_VAL_PROP_RW(int, v_padding_size);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatMenuGroup);
private:
    SDL_Texture *CreateTexture(SDL_Renderer *renderer);

    TTF_Font *font_;
    SDL_Color border_color_ = {0, 0, 0, 0};
    SDL_Color bg_color_ = {0, 0, 0, 0};
    SDL_Color hot_color_ = {0, 0, 0, 0};
    SDL_Color font_color_ = {0, 0, 0, 0};
    SDL_Texture *texture_ = nullptr;
    int h_padding_size_ = 2;
    int v_padding_size_ = 10;
    bool is_changed_ = false;
    std::vector<Column> columns_;
}; // class UIFlatMenuGroup

} // namespace utaha

#endif // UTAHA_UI_FLAT_MENU_GROUP_H_
