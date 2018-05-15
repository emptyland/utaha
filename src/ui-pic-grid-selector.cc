#include "ui-pic-grid-selector.h"
#include "glog/logging.h"
#include SDL_IMAGE_H

namespace utaha {

UIPicGridSelector::UIPicGridSelector() {
}

/*virtual*/ UIPicGridSelector::~UIPicGridSelector() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
    if (ownership_) {
        SDL_FreeSurface(pic_);
        pic_ = nullptr;
    }
}

/*virtual*/ int UIPicGridSelector::OnEvent(SDL_Event *event, bool *is_break) {
    if (!is_show() || !pic_) {
        return 0;
    }

    auto result = UIComponent::OnEvent(event, is_break);
    if (*is_break) {
        return result;
    }

    SDL_Rect rc = GetPicRect();
    if (event->type == SDL_MOUSEBUTTONDOWN) {

    }

    if (event->type == SDL_MOUSEBUTTONUP) {
        //LOG(INFO) << "button: " << (int)event->button.button;
        if (is_focused() &&
            event->button.clicks == 1 &&
            event->button.button == 1 &&
            InRect(rc, event->button.x, event->button.y)) {
            selected_x_ = (event->button.x - rc.x) / grid_size_w_;
            selected_y_ = (event->button.y - rc.y) / grid_size_h_;
        }
    }
    return 0;
}

/*virtual*/ int UIPicGridSelector::OnRender(SDL_Renderer *renderer) {
    if (!is_show() || !pic_) {
        return 0;
    }
    if (!texture_) {
        texture_ = SDL_CreateTextureFromSurface(renderer, pic_);
        if (!texture_) {
            LOG(ERROR) << "Can not create texture! " << SDL_GetError();
            return -1;
        }
    }

    SDL_Rect dst = GetPicRect(),
             src = {.x = 0, .y = 0, .w = pic_->w, .h = pic_->h};
    SDL_RenderCopy(renderer, texture_, &src, &dst);

    SDL_SetRenderDrawColor(renderer, grid_color_.r, grid_color_.g, grid_color_.b,
                           grid_color_.a);
    for (int y = dst.y; y < dst.y + dst.h + grid_size_h_; y += grid_size_h_) {
        SDL_RenderDrawLine(renderer, dst.x, y, dst.x + dst.w, y);
    }
    for (int x = dst.x; x < dst.x + dst.w + grid_size_w_; x += grid_size_w_) {
        SDL_RenderDrawLine(renderer, x, dst.y, x, dst.y + dst.h);
    }

    if (is_selected()) {
        SDL_SetRenderDrawColor(renderer, selected_color_.r, selected_color_.g,
                               selected_color_.b, selected_color_.a);
        SDL_Rect selected = {
            .x = dst.x + selected_x_ * grid_size_w_,
            .y = dst.y + selected_y_ * grid_size_h_,
            .w = grid_size_w_,
            .h = grid_size_h_,
        };
        SDL_RenderFillRect(renderer, &selected);
    }
    return UIComponent::OnRender(renderer);
}

bool UIPicGridSelector::SetPic(SDL_Surface *pic, bool ownership) {
    DCHECK_NOTNULL(pic);
    if (grid_size_h_ <= 0 || pic->h % grid_size_h_) {
        LOG(ERROR) << "Incorrect pic size or grid_size_h: " << pic->h << ", "
                   << grid_size_h_;
        return false;
    }
    if (grid_size_w_ <= 0 || pic->w % grid_size_w_) {
        LOG(ERROR) << "Incorrect pic size or grid_size_w: " << pic->w << ", "
                   << grid_size_w_;
        return false;
    }

    if (ownership_) {
        SDL_FreeSurface(pic_);
    }
    ownership_ = ownership;
    pic_ = pic;
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }

    max_h_grids_ = pic->w / grid_size_w_;
    max_v_grids_ = pic->h / grid_size_h_;
    unselected();
    return true;
}

bool UIPicGridSelector::LoadPicFromFile(const char *file_path) {
    auto pic = IMG_Load(file_path);
    if (!pic) {
        LOG(ERROR) << "Load file fail!" << SDL_GetError();
        return false;
    }
    return SetPic(pic, true);
}

SDL_Rect UIPicGridSelector::GetPicRect() const {
    SDL_Rect rc;
    rc.x = rect().x + (rect().w - pic_->w) / 2;
    rc.y = rect().y + (rect().h - pic_->h) / 2;
    rc.w = pic_->w;
    rc.h = pic_->h;
    return rc;
}

} // namespace utaha

