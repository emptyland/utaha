#include "ui-flat-pic-view.h"
#include SDL_H

namespace utaha {

UIFlatPicView::UIFlatPicView() {
}

/*virtual*/ UIFlatPicView::~UIFlatPicView() {
    if (ownership_ && pic_) {
        SDL_FreeSurface(pic_);
    }
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

/*virtual*/ int UIFlatPicView::OnEvent(SDL_Event *event, bool *is_break) {
    return UIComponent::OnEvent(event, is_break);
}

/*virtual*/ int UIFlatPicView::OnRender(SDL_Renderer *renderer) {
    if (!is_show() || !pic_) {
        return 0;
    }

    if (!texture_) {
        texture_ = SDL_CreateTextureFromSurface(renderer, pic_);
    }

    const SDL_Rect dst = {
        rect().x + padding_size_,
        rect().y + padding_size_,
        pic_->w,
        pic_->h,
    };
    SDL_RenderCopy(renderer, texture_, nullptr, &dst);

    mutable_rect()->w = pic_->w + padding_size_ * 2;
    mutable_rect()->h = pic_->h + padding_size_ * 2;
    SDL_RenderDrawRect(renderer, mutable_rect());
    return 0;
}

/*virtual*/ void UIFlatPicView::UpdateRect() {
    if (pic_) {
        mutable_rect()->w = pic_->w + padding_size_ * 2;
        mutable_rect()->h = pic_->h + padding_size_ * 2;
    } else {
        mutable_rect()->w = padding_size_ * 2;
        mutable_rect()->h = padding_size_ * 2;
    }
}

void UIFlatPicView::SetPic(SDL_Surface *pic, bool ownership) {
    if (ownership_ && pic_) {
        SDL_FreeSurface(pic_);
    }
    pic_ = pic;
    ownership_ = ownership;
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

} // namespace utaha
