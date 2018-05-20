#include "ui-flat-label.h"
#include "glog/logging.h"
#include SDL_TTF_H

namespace utaha {

UIFlatLabel::UIFlatLabel(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
}

/*virtual*/ UIFlatLabel::~UIFlatLabel() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
    }
}

/*virtual*/ int UIFlatLabel::OnEvent(SDL_Event *, bool *) {
    return 0;
}

/*virtual*/ int UIFlatLabel::OnRender(SDL_Renderer *renderer) {
    if (!is_show()) {
        return 0;
    }
    int w = 0, h = 0;
    if (!texture_) {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font_, text_.c_str(),
                                                      font_color_);
        if (!surface) {
            LOG(ERROR) << "Can not render font." << SDL_GetError();
            return -1;
        }
        w = surface->w;
        h = surface->h;

        texture_ = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture_) {
            LOG(ERROR) << "Can not create texture." << SDL_GetError();
            SDL_FreeSurface(surface);
            return -1;
        }
        SDL_FreeSurface(surface);
    } else {
        if (SDL_QueryTexture(texture_, nullptr, nullptr, &w, &h) < 0) {
            LOG(ERROR) << "Invalid texture." << SDL_GetError();
            return -1;
        }
    }

    mutable_rect()->w = w + h_padding_size_ * 2;
    mutable_rect()->h = h + v_padding_size_ * 2;

    SDL_SetRenderDrawColor(renderer, bg_color_.r, bg_color_.g, bg_color_.b,
                           bg_color_.a);
    SDL_RenderFillRect(renderer, &rect());

    SDL_SetRenderDrawColor(renderer, font_color_.r, font_color_.g,
                           font_color_.b, font_color_.a);
    SDL_Rect dst = {
        rect().x + h_padding_size_,
        rect().y + v_padding_size_,
        w,
        h,
    };
    SDL_RenderCopy(renderer, texture_, nullptr, &dst);

    SDL_SetRenderDrawColor(renderer, border_color_.r, border_color_.g,
                           border_color_.b, border_color_.a);
    SDL_RenderDrawRect(renderer, &rect());
    return 0;
}

/*virtual*/ void UIFlatLabel::UpdateRect() {
    int w = 0, h = 0;
    if (!texture_) {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font_, text_.c_str(),
                                                      font_color_);
        if (!surface) {
            LOG(ERROR) << "Can not render font." << SDL_GetError();
            return;
        }
        w = surface->w;
        h = surface->h;
        SDL_FreeSurface(surface);
    } else {
//        Uint32 format;
//        int access;
//        SDL_QueryTexture(texture_, &format, &access, &w, &h);
        SDL_QueryTexture(texture_, nullptr, nullptr, &w, &h);
    }
    w += h_padding_size_ * 2;
    h += v_padding_size_ * 2;
    mutable_rect()->w = w;
    mutable_rect()->h = h;
}

} // namespace utaha

