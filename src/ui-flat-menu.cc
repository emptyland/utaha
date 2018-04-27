#include "ui-flat-menu.h"
#include "glog/logging.h"
#include <SDL2_ttf/SDL_ttf.h>

namespace utaha {

UIFlatMenu::UIFlatMenu(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
    set_is_show(false);
}

/*virtual*/ UIFlatMenu::~UIFlatMenu() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

/*virtual*/ int UIFlatMenu::OnEvent(SDL_Event *event, bool *is_break) {
    if (!is_show()) {
        return 0;
    }

    if (event->type == SDL_MOUSEMOTION) {
        if (InRect(rect(), event->motion.x, event->motion.y)) {
            for (auto &item : items_) {
                if (event->motion.y >= item.y &&
                    event->motion.y <= item.y + item.h) {
                    item.state = Item::STATE_HOT;
                } else {
                    item.state = Item::STATE_NORMAL;
                }
            }
        } else {
            for (auto &item : items_) {
                item.state = Item::STATE_NORMAL;
            }
        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {

    }

    if (event->type == SDL_MOUSEBUTTONUP) {
        set_is_show(false);
        if (InRect(rect(), event->motion.x, event->motion.y)) {
            for (const auto &item : items_) {
                if (event->motion.y >= item.y &&
                    event->motion.y <= item.y + item.h) {
                    ProcessCmdIfNeeded(item.cmd_id, item.param, is_break);
                    if (*is_break) {
                        return 0;
                    }
                }
            }
        }
    }

    return UIComponent::OnEvent(event, is_break);
}

/*virtual*/ int UIFlatMenu::OnRender(SDL_Renderer *renderer) {
    if (!is_show() || !texture_) {
        return 0;
    }

    SDL_SetRenderDrawColor(renderer, bg_color_.r, bg_color_.g, bg_color_.b,
                           bg_color_.a);
    SDL_RenderFillRect(renderer, mutable_rect());

    for (const auto &item : items_) {
        if (item.kind == Item::MENU_ITEM) {
            if (item.state == Item::STATE_HOT) {
                SDL_SetRenderDrawColor(renderer, hot_color_.r, hot_color_.g,
                                       hot_color_.b, hot_color_.a);
                SDL_Rect item_rect = {
                    .x = rect().x,
                    .y = item.y,
                    .w = rect().w,
                    .h = item.h,
                };
                SDL_RenderFillRect(renderer, &item_rect);
            }
        } else if (item.kind == Item::MENU_DIV) {
            SDL_SetRenderDrawColor(renderer, border_color_.r, border_color_.g,
                                   border_color_.b, border_color_.a);
            SDL_RenderDrawLine(renderer, rect().x, item.y, rect().x + rect().w,
                               item.y);
        }
    }

    SDL_SetRenderDrawColor(renderer, font_color_.r, font_color_.g, font_color_.b,
                           font_color_.a);
    SDL_RenderCopy(renderer, texture_, nullptr, mutable_rect());

    SDL_SetRenderDrawColor(renderer, border_color_.r, border_color_.g,
                           border_color_.b, border_color_.a);
    SDL_RenderDrawRect(renderer, mutable_rect());
    return 0;
}

bool UIFlatMenu::Popup(int x, int y, SDL_Renderer *renderer) {
    if (is_show()) {
        return false;
    }
    if (is_changed_ && texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
    if (is_changed_) {
        is_changed_ = false;
        texture_ = CreateTexture(x, y, renderer);
        if (!texture_) {
            return false;
        }
    } else {
        mutable_rect()->x = x;
        mutable_rect()->y = y;

        int item_top = y;
        for (auto &item : items_) {
            item.y = item_top;
            item_top += item.h;
        }
    }

    set_is_show(true);
    return true;
}

UIFlatMenu *UIFlatMenu::AddItem(const char *name, int cmd_id, void *param) {
    Item item = {
        .kind        = Item::MENU_ITEM,
        .state       = Item::STATE_NORMAL,
        .name        = name,
        .cmd_id      = cmd_id,
        .param       = param,
        .is_selected = false,
    };
    items_.emplace_back(item);
    is_changed_ = true;
    return this;
}

UIFlatMenu *UIFlatMenu::AddDiv() {
    Item item = {
        .kind = Item::MENU_DIV,
    };
    items_.emplace_back(item);
    is_changed_ = true;
    return this;
}

SDL_Texture *UIFlatMenu::CreateTexture(int x, int y, SDL_Renderer *renderer) {
    std::vector<SDL_Surface *> surfaces;
    int w = 0, h = 0, item_top = y;
    for (auto &item : items_) {
        SDL_Surface *surface = nullptr;
        if (item.kind == Item::MENU_ITEM) {
            surface = TTF_RenderUTF8_Blended(font_, item.name.c_str(),
                                             font_color_);
            h += (surface->h + padding_size_ * 2);
            if (surface->w > w) {
                w = surface->w;
            }
            item.y = item_top;
            item.h = (surface->h + padding_size_ * 2);

            item_top += item.h;
        } else if (item.kind == Item::MENU_DIV) {
            h += 1;
            item.y = item_top;
            item.h = 1;

            item_top += 1;
        }
        surfaces.push_back(surface);
    }
    w += (padding_size_ * 2);

    SDL_Surface *total = SDL_CreateRGBSurface(0, w, h, 32, COLOR_R_MASK,
                                              COLOR_G_MASK, COLOR_B_MASK,
                                              COLOR_A_MASK);
    int dy = 0;
    for (auto surface : surfaces) {
        if (!surface) {
            dy += 1;
            continue;
        }
        SDL_Rect src = {
            .x = 0,
            .y = 0,
            .w = surface->w,
            .h = surface->h,
        };
        SDL_Rect dst = {
            .x = padding_size_,
            .y = dy + padding_size_,
            .w = surface->w,
            .h = surface->h,
        };
        dy += (surface->h + padding_size_ * 2);
        SDL_BlitSurface(surface, &src, total, &dst);
    }

    auto texture = SDL_CreateTextureFromSurface(renderer, total);
    for (auto surface : surfaces) {
        if (surface) {
            SDL_FreeSurface(surface);
        }
    }
    SDL_FreeSurface(total);

    mutable_rect()->x = x;
    mutable_rect()->y = y;
    mutable_rect()->w = w;
    mutable_rect()->h = h;
    return texture;
}

} // namespace utaha
