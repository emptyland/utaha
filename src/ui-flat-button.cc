#include "ui-flat-button.h"
#include "sample-render-entity.h"
#include "glog/logging.h"

namespace utaha {

UIFlatButton::UIFlatButton(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
}

/*virtual*/ UIFlatButton::~UIFlatButton() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

/*virtual*/ int UIFlatButton::OnEvent(SDL_Event *event, bool *is_break) {
    if (!is_show()) {
        return 0;
    }
    if (event->type == SDL_MOUSEMOTION) {
        if (InRect(rect(), event->button.x, event->button.y)) {
            state_ = STATE_HOT;
        } else {
            state_ = STATE_NORMAL;
        }
    }
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (InRect(rect(), event->button.x, event->button.y)) {
            state_ = STATE_PRESSED;
        } else {
            state_ = STATE_NORMAL;
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP) {
        if (InRect(rect(), event->button.x, event->button.y)) {
            state_ = STATE_HOT;
            ProcessCmdIfNeeded(cmd_id_, nullptr, is_break);
            if (*is_break) {
                return 0;
            }
        } else {
            state_ = STATE_NORMAL;
        }
    }

    return UIComponent::OnEvent(event, is_break);
}

/*virtual*/ int UIFlatButton::OnRender(SDL_Renderer *renderer) {
    if (!is_show()) {
        return 0;
    }
    if (texture_ == nullptr) {
        auto surface = TTF_RenderUTF8_Blended(font_, text_.c_str(), font_color_);
        if (!surface) {
            LOG(ERROR) << SDL_GetError();
            return -1;
        }
        texture_ = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface); surface = nullptr;
        if (!texture_) {
            LOG(ERROR) << SDL_GetError();
            return -1;
        }
    }

    switch (state_) {
        case STATE_HOT:
            SDL_SetRenderDrawColor(renderer, hot_color_.r,
                                   hot_color_.g,
                                   hot_color_.b,
                                   hot_color_.a);
            break;
        case STATE_PRESSED:
            SDL_SetRenderDrawColor(renderer, pressed_color_.r,
                                   pressed_color_.g,
                                   pressed_color_.b,
                                   pressed_color_.a);
            break;
        case STATE_NORMAL:
            SDL_SetRenderDrawColor(renderer, normal_color_.r,
                                   normal_color_.g,
                                   normal_color_.b,
                                   normal_color_.a);
            break;
        default:
            break;
    }
    SDL_RenderFillRect(renderer, mutable_rect());
    int tex_w = 0, tex_h = 0;
    SDL_QueryTexture(texture_, nullptr, nullptr, &tex_w, &tex_h);
    SDL_Rect dst = {
        rect().x + (rect().w - tex_w) / 2,
        rect().y + (rect().h - tex_h) / 2,
        tex_w,
        tex_h,
    };

    SDL_RenderCopy(renderer, texture_, nullptr, &dst);
    return 0;
}

} // namespace utaha
