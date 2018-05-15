#include "ui-flat-status-bar.h"
#include SDL_TTF_H
#include <algorithm>

namespace utaha {

UIFlatStatusBar::UIFlatStatusBar(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
}

/*virtual*/ UIFlatStatusBar::~UIFlatStatusBar() {
    for (auto grid : grids_) {
        SDL_DestroyTexture(grid->texture_);
        grid->texture_ = nullptr;
        delete grid;
    }
}

/*virtual*/ int UIFlatStatusBar::OnEvent(SDL_Event *event, bool *is_break) {
    return UIComponent::OnEvent(event, is_break);
}

/*virtual*/ int UIFlatStatusBar::OnRender(SDL_Renderer *renderer) {
    if (!is_show()) {
        return 0;
    }
    if (UpdateGridsTexture(renderer) <= 0) {
        return 0;
    }

    int x = rect().x;
    for (auto grid : grids_) {
        const SDL_Rect rc = {x, rect().y, grid->w(), rect().h};
        SDL_SetRenderDrawColor(renderer, grid->bg_color().r, grid->bg_color().g,
                               grid->bg_color().b, grid->bg_color().a);
        SDL_RenderFillRect(renderer, &rc);

        if (grid->texture_) {
            SDL_SetRenderDrawColor(renderer, grid->font_color().r,
                                   grid->font_color().g, grid->font_color().b,
                                   grid->font_color().a);
            const SDL_Rect src = {0, 0, grid->text_w_, grid->text_h_};
            SDL_Rect dst = {
                x + padding_size_,
                rect().y + (rect().h - grid->text_h_) / 2,
                grid->text_w_,
                grid->text_h_,
            };
            SDL_RenderCopy(renderer, grid->texture_, &src, &dst);
        }
        SDL_SetRenderDrawColor(renderer, border_color_.r, border_color_.g,
                               border_color_.b, border_color_.a);
        SDL_RenderDrawRect(renderer, &rc);
        x += grid->w();
    }

    SDL_SetRenderDrawColor(renderer, border_color_.r, border_color_.g,
                           border_color_.b, border_color_.a);
    SDL_RenderDrawRect(renderer, mutable_rect());
    return 0;
}

int UIFlatStatusBar::UpdateGridsTexture(SDL_Renderer *renderer) {
    int w = 0;
    for (auto grid : grids_) {
        w += grid->w();
        if (!grid->dirty_) {
            continue;
        }
        if (grid->texture_) {
            SDL_DestroyTexture(grid->texture_);
            grid->texture_ = nullptr;
        }
        if (grid->text().empty()) {
            grid->dirty_ = false;
            continue;
        }

        SDL_Surface *text = TTF_RenderUTF8_Blended(font_, grid->text().c_str(),
                                                   grid->font_color_);
        if (!text) {
            LOG(ERROR) << "Can not render text. " << SDL_GetError();
            return 0;
        }
        grid->texture_ = SDL_CreateTextureFromSurface(renderer, text);
        if (!grid->texture_) {
            LOG(ERROR) << "Can not create texture. " << SDL_GetError();
            SDL_FreeSurface(text);
            return 0;
        }
        grid->text_h_ = text->h;
        grid->text_w_ = text->w;
        SDL_FreeSurface(text);
        grid->dirty_ = false;
    }
    mutable_rect()->w = std::max(w, rect().w);
    return static_cast<int>(grids_.size());
}

} // namespace utaha
