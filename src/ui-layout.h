#ifndef UTAHA_UI_LAYOUT_H_
#define UTAHA_UI_LAYOUT_H_

#include "interactive-entity.h"
#include <vector>
#include <unordered_map>

namespace utaha {

class UILayoutRow;
class UIComponent;

//#define DEFINE_LAYOUT_ALIGNMENT(M) \
//    M(ALIGN_LEFT_OR_TOP, 0)        \
//    M(ALIGN_RIGHT_OR_BOTTOM, 1)    \
//    M(ALIGN_CENTER, 2)             \
//    M(ALIGN_FILL, 3)

class UILayout : public InteractiveEntity {
public:
    enum Alignment: int {
        ALIGN_LEFT_OR_TOP,
        ALIGN_RIGHT_OR_BOTTOM,
        ALIGN_CENTER,
        ALIGN_FILL,
    };
    typedef UILayoutRow Row;

    UILayout(SDL_Window *window);
    virtual ~UILayout();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;

    DEF_PTR_GETTER(SDL_Window, window);
    DEF_VAL_PROP_RW(int, padding_size);
    DEF_VAL_PROP_RMW(SDL_Rect, rect);
    DEF_VAL_PROP_RW(Alignment, vertical_alignment);
    DEF_VAL_PROP_RW(Alignment, horizontal_aligment);
    DEF_VAL_PROP_RW(bool, debug_mode);

    Row *AddRow(Alignment horizontal_aligment);

    size_t rows_size() const { return rows_.size(); }
    Row *row(size_t i) const { return rows_[i]; }

    friend class UILayoutRow;
    DISALLOW_IMPLICIT_CONSTRUCTORS(UILayout);
private:
    void UpdateRect();
    void UpdateRect(int w, int h);
    int CalculateMaxRowWidth();
    int CalculateRowsHeight();

    SDL_Window *window_;
    Alignment vertical_alignment_ = ALIGN_LEFT_OR_TOP;
    Alignment horizontal_aligment_ = ALIGN_LEFT_OR_TOP;
    int padding_size_ = 2;
    std::vector<Row *> rows_;
    bool debug_mode_ = false;
    SDL_Rect rect_ = {0, 0, 0, 0};
}; // class UILayout


class UILayoutRow : public InteractiveEntity {
public:
    virtual ~UILayoutRow();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;

    UILayoutRow *AddComponent(UIComponent *component);
    int CalculateWidth();
    int CalculateHeight();

    DEF_VAL_PROP_RMW(SDL_Rect, rect);
    DEF_VAL_GETTER(UILayout::Alignment, horizontal_aligment);

    size_t components_size() const { return components_.size(); }
    UIComponent *component(size_t i) const { return components_[i]; }

    friend class UILayout;
    DISALLOW_IMPLICIT_CONSTRUCTORS(UILayoutRow);
private:
    UILayoutRow(UILayout *layout, UILayout::Alignment horizontal_aligment);
    int UpdateComponentsRect(const int row_w, const int row_h, int original_x,
                             int original_y);

    UILayout *layout_;
    UILayout::Alignment horizontal_aligment_;
    std::vector<UIComponent *> components_;
    SDL_Rect rect_ = {0, 0, 0, 0};
}; // class UILayoutRow

} // namespace utaha

#endif // UTAHA_UI_LAYOUT_H_
