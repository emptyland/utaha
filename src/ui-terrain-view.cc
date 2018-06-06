#include "ui-terrain-view.h"
#include "grid-pic-storage.h"
#include "generic-storage.h"
#include "indexed-tile.h"
#include "fixed-terrain.h"
#include "base-io.h"
#include SDL_TTF_H
#include <algorithm>

namespace utaha {

UITerrainView::UITerrainView(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
}

/*virtual*/ UITerrainView::~UITerrainView() {
    for (auto tex : indexed_tex_) {
        SDL_DestroyTexture(tex);
    }
    for (auto tex : h_ruler_) {
        SDL_DestroyTexture(tex);
    }
    for (auto tex : v_ruler_) {
        SDL_DestroyTexture(tex);
    }
    if (kMiss) {
        SDL_DestroyTexture(kMiss);
        kMiss = nullptr;
    }
}

/*virtual*/ int UITerrainView::OnEvent(SDL_Event *event, bool *is_break) {
    if (!is_show() || !terrain_tiles_ || terrain_tiles_->empty()) {
        return 0;
    }

    int result = UIComponent::OnEvent(event, is_break);
    if (*is_break) {
        return result;
    }
    if (!focused()) {
        return 0;
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (mode_ == PLACE_MODE) {
            return ProcessTileSelected(event, is_break);
        }
    }

    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                if (view_port_y_ >= 0) {
                    auto old = view_port_y_;
                    view_port_y_ -= scrolling_speed_;
                    if (view_port_y_ < 0) {
                        view_port_y_ = 0;
                    }
                    ShiftRuler(&v_ruler_, view_port_y_ - old);
                }
                break;
            case SDLK_DOWN:
            case SDLK_s:
                if (view_port_y_ >= 0) {
                    auto old = view_port_y_;
                    view_port_y_ += scrolling_speed_;
                    if (view_port_y_ + view_port_v_tiles_ > max_v_tiles_) {
                        view_port_y_ = max_v_tiles_ - view_port_v_tiles_;
                    }
                    ShiftRuler(&v_ruler_, view_port_y_ - old);
                }
                break;
            case SDLK_LEFT:
            case SDLK_a:
                if (view_port_x_ >= 0) {
                    auto old = view_port_x_;
                    view_port_x_ -= scrolling_speed_;
                    if (view_port_x_ < 0) {
                        view_port_x_ = 0;
                    }
                    ShiftRuler(&h_ruler_, view_port_x_ - old);
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                if (view_port_x_ >= 0) {
                    auto old = view_port_x_;
                    view_port_x_ += scrolling_speed_;
                    if (view_port_x_ + view_port_h_tiles_ > max_h_tiles_) {
                        view_port_x_ = max_h_tiles_ - view_port_h_tiles_;
                    }
                    ShiftRuler(&h_ruler_, view_port_x_ - old);
                }
                break;
        }
    }
    return 0;
}

/*virtual*/ int UITerrainView::OnRender(SDL_Renderer *renderer) {
    if (!is_show() || !terrain_tiles_ || terrain_tiles_->empty()) {
        return 0;
    }
    UpdateRect();

    const SDL_Rect view_rc = view_rect();
    int vx, bx, vy, by, vw, vh;
    if (view_port_x_ >= 0) {
        vx = 0;
        bx = view_port_x_;
        vw = view_port_h_tiles_;
    } else {
        vx = -view_port_x_;
        bx = 0;
        vw = max_h_tiles_;
    }
    if (view_port_y_ >= 0) {
        vy = 0;
        by = view_port_y_;
        vh = view_port_v_tiles_;
    } else {
        vy = -view_port_y_;
        by = 0;
        vh = max_v_tiles_;
    }
    for (int j = 0; j < vh; ++j) {
        for (int i = 0; i < vw; ++i) {
            int x = bx + i;
            int y = by + j;
            DCHECK_GE(y * max_v_tiles_ + x, 0);
            DCHECK_LT(y * max_v_tiles_ + x, terrain_tiles_->size());
            int tile_id = (*terrain_tiles_)[y * max_h_tiles_ + x];
            if (tile_id == 0) {
                continue;
            }

            SDL_Texture *tex = nullptr;
            SDL_Rect dst = {
                view_rc.x + (i + vx) * tile_w_,
                view_rc.y + (j + vy) * tile_h_,
                tile_w_,
                tile_h_,
            };
            const IndexedTile *tile = tiles_->FindOrNull(tile_id);
            if (!tile) {
                tex = CreateOrGetMissionGrid(renderer);
                SDL_RenderCopy(renderer, tex, nullptr, &dst);
                continue;
            }
            if (!indexed_tex_[tile->tex_id()]) {
                SDL_Surface *grid = tile_tex_->FindOrNullGrid(tile->tex_id());
                if (!grid) {
                    tex = CreateOrGetMissionGrid(renderer);
                    SDL_RenderCopy(renderer, tex, nullptr, &dst);
                    continue;
                }
                indexed_tex_[tile->tex_id()] =
                    SDL_CreateTextureFromSurface(renderer, grid);
            }
            tex = indexed_tex_[tile->tex_id()];
            SDL_RenderCopy(renderer, tex, nullptr, &dst);
        }
    }

    if (has_ruler()) {
        RenderRuler(bx, by, vx, vy, vw, vh, renderer);
    }

    SDL_SetRenderDrawColor(renderer, border_color_.r, border_color_.g,
                           border_color_.b, border_color_.a);

    SDL_RenderDrawRect(renderer, &rect());


    SDL_SetRenderDrawColor(renderer, grid_color_.r, grid_color_.g,
                           grid_color_.b, grid_color_.a);
    for (int i = 0; i < vw + 1; ++i) {
        int x = view_rc.x + (vx + i) * tile_w_;
        int y = view_rc.y + vy * tile_h_;
        SDL_RenderDrawLine(renderer, x, y, x, y + vh * tile_h_);
    }
    for (int i = 0; i < vh + 1; ++i) {
        int x = view_rc.x + vx * tile_w_;
        int y = view_rc.y + (vy + i) * tile_h_;
        SDL_RenderDrawLine(renderer, x, y, x + vw * tile_w_, y);
    }
    return 0;
}

/*virtual*/ void UITerrainView::UpdateRect() {
    mutable_rect()->w = tile_w_ * view_port_h_tiles_ + padding_size_ * 2
                      + GetVRulerW();
    mutable_rect()->h = tile_h_ * view_port_v_tiles_ + padding_size_ * 2
                      + GetHRulerH();
}

bool UITerrainView::InvalidateWhole() {
    for (auto tex : indexed_tex_) {
        SDL_DestroyTexture(tex);
    }
    indexed_tex_.clear();
    for (auto tex : h_ruler_) {
        SDL_DestroyTexture(tex);
    }
    h_ruler_.clear();
    h_ruler_.resize(std::min(max_h_tiles_, view_port_h_tiles_), nullptr);
    kMaxHRulerH = 0;
    for (auto tex : v_ruler_) {
        SDL_DestroyTexture(tex);
    }
    v_ruler_.clear();
    v_ruler_.resize(std::min(max_v_tiles_, view_port_v_tiles_), nullptr);
    kMaxVRulerW = 0;

    view_port_x_ = (max_h_tiles_ - view_port_h_tiles_) / 2;
    view_port_y_ = (max_v_tiles_ - view_port_v_tiles_) / 2;
    indexed_tex_.resize(tile_tex_->grid_pics().size(), nullptr);
    return true;
}

int UITerrainView::RenderRuler(int bx, int by, int vx, int vy, int vw, int vh,
                               SDL_Renderer *renderer) {
    for (int i = 0; i < h_ruler_.size(); ++i) {
        if (!h_ruler_[i]) {
            SDL_Surface *item = TTF_RenderUTF8_Blended(font_,
                Original::sprintf("%d", bx + i).c_str(), font_color_);
            h_ruler_[i] = SDL_CreateTextureFromSurface(renderer, item);
        }

        int w = 0, h = 0;
        SDL_QueryTexture(h_ruler_[i], nullptr, nullptr, &w, &h);
        SDL_Rect rd = {
            rect().x + padding_size_ + GetVRulerW() + (i + vx) * tile_w_
                     + (tile_w_ - w) / 2,
            rect().y + padding_size_,
            w,
            h
        };
        SDL_RenderCopy(renderer, h_ruler_[i], nullptr, &rd);
    }

    for (int i = 0; i < v_ruler_.size(); ++i) {
        if (!v_ruler_[i]) {
            SDL_Surface *item = TTF_RenderUTF8_Blended(font_,
                Original::sprintf("%d", by + i).c_str(), font_color_);
            v_ruler_[i] = SDL_CreateTextureFromSurface(renderer, item);
        }

        int w = 0, h = 0;
        SDL_QueryTexture(v_ruler_[i], nullptr, nullptr, &w, &h);
        SDL_Rect rd = {
            rect().x + padding_size_,
            rect().y + padding_size_ + GetHRulerH() + (i + vy) * tile_h_
                     + (tile_h_ - h) / 2,
            w,
            h
        };
        SDL_RenderCopy(renderer, v_ruler_[i], nullptr, &rd);
    }
    return 0;
}

SDL_Texture *UITerrainView::CreateOrGetMissionGrid(SDL_Renderer *renderer) {
    if (kMiss) {
        return kMiss;
    }
    SDL_Surface *miss = SDL_CreateRGBSurfaceWithFormat(0, tile_w_, tile_h_, 32,
                                                       SDL_PIXELFORMAT_RGBA8888);
    if (!miss) {
        LOG(ERROR) << "Can not create surface. " << SDL_GetError();
        return nullptr;
    }
    SDL_FillRect(miss, nullptr, SDL_MapRGBA(miss->format, 0xff, 0, 0, 0xff));
    kMiss = SDL_CreateTextureFromSurface(renderer, miss);
    SDL_FreeSurface(miss);
    return kMiss;
}

int UITerrainView::ShiftRuler(std::vector<SDL_Texture *> *ruler, int n) {
    if (n < 0) {
        if (-n >= ruler->size()) {
            for (int i = 0; i < ruler->size(); ++i) {
                SDL_DestroyTexture(ruler->at(i));
                ruler->at(i) = nullptr;
            }
        } else {
            for (int i = static_cast<int>(ruler->size()) + n; i < ruler->size();
                 ++i) {
                SDL_DestroyTexture(ruler->at(i));
            }
            ::memmove(&ruler->at(-n), &ruler->at(0),
                      (ruler->size() + n) * sizeof(SDL_Texture *));
            for (int i = 0; i < -n; ++i) {
                ruler->at(i) = nullptr;
            }
        }
    } if (n > 0) {
        if (n >= ruler->size()) {
            for (int i = 0; i < ruler->size(); ++i) {
                SDL_DestroyTexture(ruler->at(i));
                ruler->at(i) = nullptr;
            }
        } else {
            for (int i = 0; i < n; ++i) {
                SDL_DestroyTexture(ruler->at(i));
                ruler->at(i) = nullptr;
            }
            ::memmove(&ruler->at(0), &ruler->at(n),
                (ruler->size() - n) * sizeof(SDL_Texture *));
            for (int i = static_cast<int>(ruler->size()) - n; i < ruler->size();
                 ++i) {
                ruler->at(i) = nullptr;
            }

        }
    }
    return 0;
}

int UITerrainView::GetHRulerH() const {
    if (kMaxHRulerH) {
        return kMaxHRulerH;
    }
    SDL_Surface *max_ruler_item = TTF_RenderText_Blended(font_, 
        Original::sprintf("%d", max_h_tiles_).c_str(), font_color_);
    if (!max_ruler_item) {
        LOG(ERROR) << "Can not create surface." << SDL_GetError();
        return 0;
    }
    kMaxHRulerH = max_ruler_item->h;
    SDL_FreeSurface(max_ruler_item);
    return kMaxHRulerH;
}

int UITerrainView::GetVRulerW() const {
    if (kMaxVRulerW) {
        return kMaxVRulerW;
    }
    SDL_Surface *max_ruler_item = TTF_RenderText_Blended(font_,
        Original::sprintf("%d", max_v_tiles_).c_str(), font_color_);
    if (!max_ruler_item) {
        LOG(ERROR) << "Can not create surface." << SDL_GetError();
        return 0;
    }
    kMaxVRulerW = max_ruler_item->w;
    SDL_FreeSurface(max_ruler_item);
    return kMaxVRulerW;
}

int UITerrainView::ProcessTileSelected(SDL_Event *e, bool *is_break) {
    SDL_Rect view_rc = view_rect();

    if (!InRect(view_rc, e->button.x, e->button.y)) {
        return 0;
    }

    int x = (e->button.x - view_rc.x) / tile_w_;
    int y = (e->button.y - view_rc.y) / tile_h_;
    x = view_port_x_ + x;
    y = view_port_y_ + y;
    if (x >= 0 && x < max_h_tiles_ &&
        y >= 0 && y < max_v_tiles_) {
        TerrainViewEvent e;
        e.event = TerrainViewEvent::TILE_SELECTED;
        e.tile.x = x;
        e.tile.y = y;
        int rv = ProcessCmdIfNeeded(cmd_id_, &e, is_break);
        if (*is_break) {
            return rv;
        }
        return 1;
    }
    return 0;
}

} // namespace utaha
