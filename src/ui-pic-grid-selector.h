#ifndef UTAHA_UI_PIC_GRID_SELECTOR_H_
#define UTAHA_UI_PIC_GRID_SELECTOR_H_

#include "ui-component.h"
#include <tuple>

namespace utaha {

class UIPicGridSelector : public UIComponent {
public:
    UIPicGridSelector();
    virtual ~UIPicGridSelector();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;

    DEF_VAL_PROP_RW(int, cmd_id);
    DEF_VAL_GETTER(int, max_h_grids);
    DEF_VAL_GETTER(int, max_v_grids);
    DEF_VAL_PROP_RMW(SDL_Color, grid_color);
    DEF_VAL_PROP_RMW(SDL_Color, selected_color);
    DEF_VAL_PROP_RW(int, grid_size_w);
    DEF_VAL_PROP_RW(int, grid_size_h);
    DEF_VAL_GETTER(int, selected_x);
    DEF_VAL_GETTER(int, selected_y);
    DEF_PTR_GETTER(SDL_Surface, pic);

    bool is_selected() const { return selected_y_ > -1 && selected_x_ > -1; }
    void unselected() { selected_x_ = -1; selected_y_ = -1; }

    int selected_index() { return selected_index(selected_x_, selected_y_); }

    int selected_index(int x, int y) { return y * max_h_grids_ + x; }

    bool SetPic(SDL_Surface *pic, bool ownership);
    bool LoadPicFromFile(const char *file_path);

    SDL_Surface *CutSelectedSurface(int clipping) {
        return !is_selected() ? nullptr :
               CutSelectedSurface(selected_x_, selected_y_, clipping);
    }
    SDL_Surface *CutSelectedSurface(int x, int y, int clipping);

    SDL_Rect GetPicRect() const;
private:
    int cmd_id_ = 0;
    SDL_Color grid_color_ = {0, 0, 0, 0};
    SDL_Color selected_color_ = {0, 0, 0, 0};
    SDL_Texture *texture_ = nullptr;
    SDL_Surface *pic_ = nullptr;
    int grid_size_w_ = 0;
    int grid_size_h_ = 0;
    int max_h_grids_ = 0;
    int max_v_grids_ = 0;
    int selected_x_ = -1;
    int selected_y_ = -1;
    bool ownership_ = false;
}; // class UIPicGridSelector

} // namespace utaha

#endif // UTAHA_UI_PIC_GRID_SELECTOR_H_
