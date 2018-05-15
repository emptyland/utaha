#ifndef UTAHA_UI_COMPONENT_H_
#define UTAHA_UI_COMPONENT_H_

#include "interactive-entity.h"
#include <stdint.h>
#include <string>

namespace utaha {

class UIComponent : public InteractiveEntity {
public:
    UIComponent();
    virtual ~UIComponent();
    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual void UpdateRect();

    DEF_VAL_PROP_RW(int64_t, id);
    DEF_VAL_PROP_RW(std::string, name);
    DEF_VAL_PROP_RW(bool, is_show);
    DEF_VAL_PROP_RW(bool, is_focused);
    DEF_VAL_PROP_RMW(SDL_Rect, rect);

    int ProcessCmdIfNeeded(int cmd_id, void *param, bool *is_break);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIComponent);
private:
    int64_t id_ = 0;
    std::string name_;
    bool is_show_ = true;
    bool is_focused_ = false;
    SDL_Rect rect_;
};

inline bool InRect(const SDL_Rect &rect, int x, int y) {
    return x >= rect.x && x <= rect.x + rect.w &&
           y >= rect.y && y <= rect.y + rect.h;
}

inline SDL_Color ToColor(int color) {
    return SDL_Color {
        static_cast<Uint8>((color & 0xff000000) >> 24),
        static_cast<Uint8>((color & 0x00ff0000) >> 16),
        static_cast<Uint8>((color & 0x0000ff00) >> 8),
        static_cast<Uint8>(color & 0x000000ff),
    };
}

} // namespace utaha

#endif // UTAHA_UI_COMPONENT_H_
