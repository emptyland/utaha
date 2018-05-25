#include "ui-animated-avatar-view.h"
#include "glog/logging.h"
#include SDL_TTF_H

namespace utaha {

static const char *kDirNames[MAX_DIR] = {
    "N", "S", "W", "E",
};

UIAnimatedAvatarView::UIAnimatedAvatarView(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
    for (int i = 0; i < MAX_DIR; ++i) {
        for (int j = 0; j < AnimatedAvatar::Animation::kNumFrames; ++j) {
            frame_[i][j] = nullptr;
        }
    }
    memset(labels_, 0, arraysize(labels_) * sizeof(SDL_Texture*));
}

/*virtual*/ UIAnimatedAvatarView::~UIAnimatedAvatarView() {
    if (ownership_) {
        delete avatar_;
        avatar_ = nullptr;
    }

    for (int i = 0; i < MAX_DIR; ++i) {
        for (int j = 0; j < AnimatedAvatar::Animation::kNumFrames; ++j) {
            if (frame_[i][j]) {
                SDL_DestroyTexture(frame_[i][j]);
                frame_[i][j] = nullptr;
            }
        }

        if (labels_[i]) {
            SDL_DestroyTexture(labels_[i]);
            labels_[i] = nullptr;
        }
    }
}

/*virtual*/
int UIAnimatedAvatarView::OnEvent(SDL_Event *event, bool *is_break) {
    return UIComponent::OnEvent(event, is_break);
}

/*virtual*/ int UIAnimatedAvatarView::OnRender(SDL_Renderer *renderer) {
    if (!is_show()) {
        return 0;
    }
    if (!CreateLabelsIfNeeded(renderer)) {
        return -1;
    }
    if (!CreateFramesIfNeeded(renderer)) {
        return -1;
    }

    int max_label_h = 0;
    int labels_w[MAX_DIR], labels_h[MAX_DIR];
    for (int i = 0; i < MAX_DIR; ++i) {
        SDL_QueryTexture(labels_[i], nullptr, nullptr, &labels_w[i], &labels_h[i]);
        if (labels_h[i] > max_label_h) {
            max_label_h = labels_h[i];
        }
    }

    mutable_rect()->w = (view_w_ + padding_size_ * 2) * MAX_DIR
                      + padding_size_ * (MAX_DIR - 1);
    mutable_rect()->h = max_label_h + view_h_ + padding_size_ * 4;

    int x = rect().x;
    for (int i = 0; i < MAX_DIR; ++i) {
        SDL_Rect dst = {
            x + padding_size_,
            rect().y + max_label_h + padding_size_ * 3,
            view_w_,
            view_h_,
        };
        if (frame_[i][i_frame_]) {
            SDL_RenderCopy(renderer, frame_[i][i_frame_], nullptr, &dst);
        }

        SDL_SetRenderDrawColor(renderer, border_color_.r, border_color_.g,
                               border_color_.b, border_color_.a);
        dst = {
            x,
            rect().y + max_label_h + padding_size_ * 2,
            view_w_ + padding_size_ * 2,
            view_h_ + padding_size_ * 2,
        };
        SDL_RenderDrawRect(renderer, &dst);

        SDL_SetRenderDrawColor(renderer, font_color_.r, font_color_.g,
                               font_color_.b, font_color_.a);
        dst = {
            x + (dst.w - labels_w[i]) / 2,
            rect().y + padding_size_,
            labels_w[i],
            labels_h[i],
        };
        SDL_RenderCopy(renderer, labels_[i], nullptr, &dst);

        x += view_w_ + padding_size_ * 3;
    }

    if (SDL_GetTicks() - latest_tick_ >= animated_speed_) {
        i_frame_ = (i_frame_ + 1) % AnimatedAvatar::Animation::kNumFrames;
        latest_tick_ = SDL_GetTicks();
    }
//    if (SDL_GetTicks() % animated_speed_ == 0) {
//        i_frame_ = (i_frame_ + 1) % AnimatedAvatar::Animation::kNumFrames;
//    }
    return UIComponent::OnRender(renderer);
}

/*virtual*/ void UIAnimatedAvatarView::UpdateRect() {
    int max_label_h = 0;
    int labels_w[MAX_DIR], labels_h[MAX_DIR];
    for (int i = 0; i < MAX_DIR; ++i) {
        auto surface = TTF_RenderUTF8_Blended(font_, kDirNames[i],
                                              font_color_);
        if (!surface) {
            LOG(ERROR) << "Can not create surface! " << SDL_GetError();
            return;
        }
        labels_w[i] = surface->w;
        labels_h[i] = surface->h;
        if (labels_h[i] > max_label_h) {
            max_label_h = labels_w[i];
        }

        SDL_FreeSurface(surface);
    }
    mutable_rect()->w = (view_w_ + padding_size_ * 2) * MAX_DIR
                      + padding_size_ * (MAX_DIR - 1);
    mutable_rect()->h = max_label_h + view_h_ + padding_size_ * 4;
}

void UIAnimatedAvatarView::SetAvatar(AnimatedAvatar *avatar, bool ownership) {
    if (ownership_) {
        delete avatar_;
        avatar_ = nullptr;
    }

    avatar_ = avatar;
    ownership_ = ownership;
    for (int i = 0; i < MAX_DIR; ++i) {
        for (int j = 0; j < AnimatedAvatar::Animation::kNumFrames; ++j) {
            if (frame_[i][j]) {
                SDL_DestroyTexture(frame_[i][j]);
                frame_[i][j] = nullptr;
            }
        }
    }
    changed_ = true;
}

bool UIAnimatedAvatarView::CreateFramesIfNeeded(SDL_Renderer *renderer) {
    if (!avatar_) {
        return true;
    }
    DCHECK_NOTNULL(grids_);
    for (int i = 0; i < MAX_DIR; ++i) {
        for (int j = 0; j < AnimatedAvatar::Animation::kNumFrames; ++j) {
            if (frame_[i][j]) {
                continue;
            }

            auto tex_id = avatar_->frame(static_cast<Direction>(i), j);
            DCHECK_GE(tex_id, 0);
            DCHECK_LT(tex_id, grids_->size());
            frame_[i][j] = SDL_CreateTextureFromSurface(renderer,
                                                        grids_->at(tex_id));
            if (!frame_[i][j]) {
                LOG(ERROR) << "Can not create texture for frame."
                           << SDL_GetError();
                return false;
            }
        }
    }
    return true;
}

bool UIAnimatedAvatarView::CreateLabelsIfNeeded(SDL_Renderer *renderer) {
    if (labels_[0]) {
        return true;
    }

    for (int i = 0; i < MAX_DIR; ++i) {
        auto surface = TTF_RenderUTF8_Blended(font_, kDirNames[i],
                                              font_color_);
        if (!surface) {
            LOG(ERROR) << "Can not create surface! " << SDL_GetError();
            return false;
        }
        labels_[i] = SDL_CreateTextureFromSurface(renderer, surface);
        if (!labels_[i]) {
            LOG(ERROR) << "Can not create texture! " << SDL_GetError();

            for (int j = 0; j < i; j++) {
                SDL_DestroyTexture(labels_[j]);
                labels_[j] = nullptr;
            }
            SDL_FreeSurface(surface);
            return false;
        }
        SDL_FreeSurface(surface);
    }
    return true;
}

} // namespace utaha
