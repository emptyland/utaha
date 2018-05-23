#include "ui-flat-menu-group.h"
#include "ui-flat-menu.h"
#include "glog/logging.h"
#include SDL_TTF_H

namespace utaha {

UIFlatMenuGroup::UIFlatMenuGroup(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
}

/*virtual*/ UIFlatMenuGroup::~UIFlatMenuGroup() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
    for (const auto &column : columns_) {
        delete column.menu;
    }
}

/*virtual*/ int UIFlatMenuGroup::OnEvent(SDL_Event *event, bool *is_break) {
    if (!is_show()) {
        return 0;
    }

    int result = UIComponent::OnEvent(event, is_break);
    if (*is_break) {
        return result;
    }

    if (event->type == SDL_MOUSEMOTION ||
        event->type == SDL_MOUSEBUTTONDOWN) {
        if (focused() &&
            InRect(rect(), event->button.x, event->button.y)) {
            for (auto &column : columns_) {
                if (event->button.x >= column.x &&
                    event->button.x <= column.x + column.w) {
                    column.is_hot = true;
                } else {
                    column.is_hot = false;
                }
            }
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP) {
        if (focused() &&
            InRect(rect(), event->button.x, event->button.y)) {
            for (const auto &column : columns_) {
                if (event->button.x >= column.x &&
                    event->button.x <= column.x + column.w && column.is_hot) {
                    ProcessCmdIfNeeded(column.cmd_id, column.menu, is_break);
                    if (*is_break) {
                        return 0;
                    }
                    return 1;
                }
            }
        }
    }

    for (const auto &column : columns_) {
        if (column.menu) {
            result = column.menu->OnEvent(event, is_break);
            if (*is_break) {
                return 0;
            }
            if (result) {
                return result;
            }
        }
    }
    return 0;
}

/*virtual*/ int UIFlatMenuGroup::OnRender(SDL_Renderer *renderer) {
    if (!is_show()) {
        return 0;
    }
    if (is_changed_) {
        if (texture_) {
            SDL_DestroyTexture(texture_);
            texture_ = nullptr;
        }
        texture_ = CreateTexture(renderer);
        is_changed_ = false;
    } else {
        int x = rect().x;
        for (auto &column : columns_) {
            column.x = x;
            x += column.w;
        }
    }
    if (!texture_) {
        return 0;
    }

    SDL_SetRenderDrawColor(renderer, bg_color_.r, bg_color_.g, bg_color_.b,
                           bg_color_.a);
    SDL_RenderFillRect(renderer, mutable_rect());
    for (const auto &column : columns_) {
        if (focused() && column.is_hot) {
            SDL_SetRenderDrawColor(renderer, hot_color_.r, hot_color_.g,
                                   hot_color_.b, hot_color_.a);
            SDL_Rect col_rect = {
                column.x,
                rect().y,
                column.w,
                rect().h,
            };
            SDL_RenderFillRect(renderer, &col_rect);
            if (column.menu) {
                column.menu->Popup(column.x, rect().y + rect().h, renderer);
            }
        } else {
            if (column.menu) {
                column.menu->set_is_show(false);
            }
        }
        if (column.menu) {
            column.menu->OnRender(renderer);
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

/*virtual*/ void UIFlatMenuGroup::UpdateRect() {
    int h = 0, w = 0;
    for (const auto &column : columns_) {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font_, column.name.c_str(),
                                                      font_color_);
        if (!surface) {
            LOG(ERROR) << "Update rect fail!" << SDL_GetError();
            break;
        }

        if (surface->h > h) {
            h = surface->h;
        }
        w += surface->w + h_padding_size_ * 2;
        SDL_FreeSurface(surface);
    }
    h += v_padding_size_ * 2;

    mutable_rect()->w = w;
    mutable_rect()->h = h;
}

void UIFlatMenuGroup::AddColumn(const char *name, int cmd_id, UIFlatMenu *menu) {
    Column column = {
        false,
        DCHECK_NOTNULL(name),
        cmd_id,
        0,
        0,
        menu,
    };

    columns_.emplace_back(column);
    is_changed_ = true;
}

SDL_Texture *UIFlatMenuGroup::CreateTexture(SDL_Renderer *renderer) {
    std::vector<SDL_Surface *> surfaces;
    SDL_Texture *texture = nullptr;
    SDL_Surface *whole = nullptr;

    int x = rect().x, h = 0, w = 0;
    for (auto &column : columns_) {
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font_, column.name.c_str(),
                                                      font_color_);
        if (!surface) {
            goto cleanup;
        }
        column.x = x;
        column.w = surface->w + h_padding_size_ * 2;
        if (surface->h > h) {
            h = surface->h;
        }
        x += column.w;
        w += column.w;

        surfaces.push_back(surface);
    }
    h += v_padding_size_ * 2;

    whole = SDL_CreateRGBSurface(0, w, h, 32, COLOR_R_MASK, COLOR_G_MASK,
                                 COLOR_B_MASK, COLOR_A_MASK);
    if (!whole) {
        goto cleanup;
    }
    x = 0;
    for (auto surface : surfaces) {
        SDL_Rect dst = {
            x + h_padding_size_,
            v_padding_size_,
            surface->w,
            surface->h,
        };
        SDL_Rect src = {
            0,
            0,
            surface->w,
            surface->h,
        };
        SDL_BlitSurface(surface, &src, whole, &dst);
        x += (surface->w + h_padding_size_ * 2);
    }
    texture = SDL_CreateTextureFromSurface(renderer, whole);
    if (!texture) {
        goto cleanup;
    }

    mutable_rect()->w = w;
    mutable_rect()->h = h;
cleanup:
    for (auto surface : surfaces) {
        SDL_FreeSurface(surface);
    }
    if (whole) {
        SDL_FreeSurface(whole);
    }
    return texture;
}

} // namespace utaha
