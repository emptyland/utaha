#include "ui-flat-check-box.h"
#include "glog/logging.h"
#include SDL_TTF_H

namespace utaha {

UIFlatCheckBox::UIFlatCheckBox(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
}

/*virtual*/ UIFlatCheckBox::~UIFlatCheckBox() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

/*virtual*/ int UIFlatCheckBox::OnEvent(SDL_Event *event, bool *is_break) {
    if (!is_show()) {
        return 0;
    }

    auto result = UIComponent::OnEvent(event, is_break);
    if (*is_break) {
        return result;
    }

    if (event->type == SDL_MOUSEBUTTONUP) {
        if (is_focused() && InRect(rect(), event->button.x, event->button.y)) {
            checked_ = !checked_;
        }
    }
    return UIComponent::OnEvent(event, is_break);
}

/*virtual*/ int UIFlatCheckBox::OnRender(SDL_Renderer *renderer) {
    if (!is_show()) {
        return 0;
    }

    if (!texture_) {
        SDL_Surface *surface = CreateSurface();
        texture_ = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture_) {
            LOG(ERROR) << "Can not create texture!" << SDL_GetError();
            SDL_FreeSurface(surface);
            return -1;
        }
        SDL_FreeSurface(surface);
    }

    SDL_SetRenderDrawColor(renderer, box_color_.r, box_color_.g, box_color_.b,
                           box_color_.a);
    SDL_Rect box_rc = {
        .x = rect().x + h_padding_size_,
        .y = rect().y + (rect().h - box_size_) / 2,
        .w = box_size_,
        .h = box_size_,
    };
    SDL_RenderDrawRect(renderer, &box_rc);
    if (checked_) {
        box_rc.x += 2;
        box_rc.y += 2;
        box_rc.w -= 4;
        box_rc.h -= 4;
        SDL_RenderFillRect(renderer, &box_rc);
    } else {
        // Nothing
    }

    const SDL_Rect src = {.x = 0, .y = 0, .w = label_w_, .h = label_h_};
    SDL_Rect dst = {
        .x = rect().x + h_padding_size_ * 2 + box_size_,
        .y = rect().y + v_padding_size_,
        .w = label_w_,
        .h = label_h_,
    };
    SDL_SetRenderDrawColor(renderer, font_color_.r, font_color_.g, font_color_.b,
                           font_color_.a);
    SDL_RenderCopy(renderer, texture_, &src, &dst);
    
    return UIComponent::OnRender(renderer);
}

/*virtual*/ void UIFlatCheckBox::UpdateRect() {
    auto surface = CreateSurface();
    if (surface) {
        SDL_FreeSurface(surface);
    }
}

SDL_Surface *UIFlatCheckBox::CreateSurface() {
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font_, label_.c_str(),
                                                  font_color_);
    if (!surface) {
        LOG(ERROR) << "Can not create ttf surface!" << SDL_GetError();
        return nullptr;
    }
    label_w_ = surface->w;
    label_h_ = surface->h;
    mutable_rect()->h = label_h_ + v_padding_size_ * 2;
    box_size_ = label_h_ / 3 * 2;
    mutable_rect()->w = box_size_ + h_padding_size_ + label_w_ + h_padding_size_ * 2;
    return surface;
}

} // namespace utaha

