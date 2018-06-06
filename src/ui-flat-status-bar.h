#ifndef UTAHA_FLAT_STATUS_BAR_H_
#define UTAHA_FLAT_STATUS_BAR_H_

#include "ui-component.h"
#include "glog/logging.h"
#include <vector>
#include <string>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class UIFlatStatusBar;

class UIFlatStatusBarGrid {
public:
    UIFlatStatusBarGrid(const std::string &text, int index, int w)
        : index_(index), w_(w), text_(text) {}

    DEF_VAL_GETTER(int, index);
    DEF_VAL_PROP_RW(int, w);
    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RMW(SDL_Color, bg_color);
    DEF_VAL_PROP_RMW(std::string, text)

    friend class UIFlatStatusBar;
private:
    UIFlatStatusBarGrid *Dirty() { dirty_ = true; return this; }

    int index_;
    int w_;
    SDL_Color font_color_;
    SDL_Color bg_color_;
    std::string text_;
    SDL_Texture *texture_ = nullptr;
    int text_w_ = 0;
    int text_h_ = 0;
    bool dirty_ = true;
}; // class UIFlatStatusBarGrid

class UIFlatStatusBar : public UIComponent {
public:
    typedef UIFlatStatusBarGrid Grid;

    UIFlatStatusBar(TTF_Font *font);
    virtual ~UIFlatStatusBar();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;

    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RMW(SDL_Color, bg_color);
    DEF_VAL_PROP_RW(int, padding_size);

    inline Grid *AddGrid(const char *text, int w);
    inline Grid *mutable_grid(int index);
    inline const Grid &grid(int index) const;

    inline void SetGridText(int index, const std::string &text,
                            const SDL_Color &bg_color);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatStatusBar);
private:
    int UpdateGridsTexture(SDL_Renderer *renderer);

    TTF_Font *font_;
    SDL_Color border_color_ = {0, 0, 0, 0};
    SDL_Color font_color_ = {0, 0, 0, 0};
    SDL_Color bg_color_ = {0, 0, 0, 0};
    int padding_size_ = 10;
    std::vector<Grid *> grids_;
}; // class UIFlatStatusBar

inline UIFlatStatusBarGrid *UIFlatStatusBar::AddGrid(const char *text, int w) {
    auto grid = new UIFlatStatusBarGrid(text, static_cast<int>(grids_.size()),
                                        w);
    grid->set_bg_color(bg_color_);
    grid->set_font_color(font_color_);
    grids_.push_back(grid);
    return grid;
}

inline UIFlatStatusBarGrid *UIFlatStatusBar::mutable_grid(int index) {
    DCHECK_GE(index, 0);
    DCHECK_LT(index, grids_.size());
    return grids_.at(index)->Dirty();
}

inline const UIFlatStatusBarGrid &UIFlatStatusBar::grid(int index) const {
    DCHECK_GE(index, 0);
    DCHECK_LT(index, grids_.size());
    return *grids_.at(index);
}

inline void UIFlatStatusBar::SetGridText(int index, const std::string &text,
                                         const SDL_Color &bg_color) {
    mutable_grid(index)->set_bg_color(bg_color);
    mutable_grid(index)->set_text(text);
}

} // namespace utaha

#endif // UTAHA_FLAT_STATUS_BAR_H_
