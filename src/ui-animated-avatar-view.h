#ifndef UTAHA_UI_ANIMATED_AVATAR_VIEW_H_
#define UTAHA_UI_ANIMATED_AVATAR_VIEW_H_

#include "animated-avatar.h"
#include "ui-component.h"
#include <vector>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class UIAnimatedAvatarView : public UIComponent {
public:
    UIAnimatedAvatarView(TTF_Font *font);
    virtual ~UIAnimatedAvatarView();

    DEF_VAL_PROP_RW(int, padding_size);
    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RW(int, view_w);
    DEF_VAL_PROP_RW(int, view_h);
    DEF_VAL_PROP_RW(int, animated_speed);
    DEF_PTR_PROP_RW_NOTNULL2(const std::vector<SDL_Surface *>, grids);

    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;
    virtual void UpdateRect() override;

    void SetAvatar(AnimatedAvatar *avatar, bool ownership);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIAnimatedAvatarView);
private:
    bool CreateFramesIfNeeded(SDL_Renderer *renderer);
    bool CreateLabelsIfNeeded(SDL_Renderer *renderer);

    TTF_Font *font_;
    int padding_size_ = 10;
    SDL_Color font_color_ = {0, 0, 0, 0};
    SDL_Color border_color_ = {0, 0, 0, 0};
    int view_w_ = 20;
    int view_h_ = 48;
    int animated_speed_ = 1000;
    const AnimatedAvatar *avatar_ = nullptr;
    bool ownership_ = false;
    bool changed_ = false;
    int i_frame_ = 0;
    SDL_Texture *frame_[MAX_DIR][AnimatedAvatar::Animation::kNumFrames];
    SDL_Texture *labels_[MAX_DIR] = {0};
    const std::vector<SDL_Surface *> *grids_ = nullptr;
}; // class UIAnimatedAvatarView

} // namespace utaha

#endif // UTAHA_UI_ANIMATED_AVATAR_VIEW_H_
