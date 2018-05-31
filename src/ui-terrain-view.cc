#include "ui-terrain-view.h"
#include "grid-pic-storage.h"
#include "generic-storage.h"
#include "indexed-tile.h"
#include "fixed-terrain.h"
#include "base-io.h"
#include SDL_TTF_H

namespace utaha {

UITerrainView::UITerrainView(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
}

/*virtual*/ UITerrainView::~UITerrainView() {
    for (auto tex : indexed_tex_) {
        SDL_DestroyTexture(tex);
    }
    if (h_ruler_) {
        SDL_DestroyTexture(h_ruler_);
        h_ruler_ = nullptr;
    }
    if (v_ruler_) {
        SDL_DestroyTexture(v_ruler_);
        v_ruler_ = nullptr;
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

    //SDL_Rect view_rc = view_rect();
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
                    view_port_y_ -= scrolling_speed_;
                    if (view_port_y_ < 0) {
                        view_port_y_ = 0;
                    }
                }
                break;
            case SDLK_DOWN:
            case SDLK_s:
                if (view_port_y_ >= 0) {
                    view_port_y_ += scrolling_speed_;
                    if (view_port_y_ + view_port_v_tiles_ > max_v_tiles_) {
                        view_port_y_ = max_v_tiles_ - view_port_v_tiles_;
                    }
                }
                break;
            case SDLK_LEFT:
            case SDLK_a:
                if (view_port_x_ >= 0) {
                    view_port_x_ -= scrolling_speed_;
                    if (view_port_x_ < 0) {
                        view_port_x_ = 0;
                    }
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                if (view_port_x_ >= 0) {
                    view_port_x_ += scrolling_speed_;
                    if (view_port_x_ + view_port_h_tiles_ > max_h_tiles_) {
                        view_port_x_ = max_h_tiles_ - view_port_h_tiles_;
                    }
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
    if (h_ruler_) {
        SDL_DestroyTexture(h_ruler_);
        h_ruler_ = nullptr;
    }
    if (v_ruler_) {
        SDL_DestroyTexture(v_ruler_);
        v_ruler_ = nullptr;
    }

    view_port_x_ = (max_h_tiles_ - view_port_h_tiles_) / 2;
    view_port_y_ = (max_v_tiles_ - view_port_v_tiles_) / 2;
    indexed_tex_.resize(tile_tex_->grid_pics().size(), nullptr);
    return true;
}

int UITerrainView::RenderRuler(int bx, int by, int vx, int vy, int vw, int vh,
                               SDL_Renderer *renderer) {
    if (!h_ruler_) {
        SDL_Surface *surface = CreateHRulerSurface(max_h_tiles_, tile_w_);
        h_ruler_ = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    SDL_Rect rs = {bx * tile_w_, 0, vw * tile_w_, GetHRulerH()};
    SDL_Rect rd = {
        rect().x + padding_size_ + GetVRulerW() + vx * tile_w_,
        rect().y + padding_size_,
        rs.w,
        rs.h
    };
    SDL_RenderCopy(renderer, h_ruler_, &rs, &rd);

    if (!v_ruler_) {
        SDL_Surface *surface = CreateVRulerSurface(max_v_tiles_, tile_h_);
        v_ruler_ = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    rs = {0, by * tile_h_, GetVRulerW(), vh * tile_h_};
    rd = {
        rect().x + padding_size_,
        rect().y + padding_size_ + GetHRulerH() + vy * tile_h_,
        rs.w,
        rs.h,
    };
    SDL_RenderCopy(renderer, v_ruler_, &rs, &rd);
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
    SDL_LockSurface(miss);
    Uint32 *pixels = static_cast<Uint32 *>(miss->pixels);
    for (int i = 0; i < miss->w * miss->h; ++i) {
        pixels[i] = SDL_MapRGBA(miss->format, 0xff, 0, 0, 0xff);
    }
    SDL_UnlockSurface(miss);
    kMiss = SDL_CreateTextureFromSurface(renderer, miss);
    SDL_FreeSurface(miss);
    return kMiss;
}

SDL_Surface *UITerrainView::CreateHRulerSurface(int max_h_tiles,
                                                int tile_w) const {
    DCHECK_GT(max_h_tiles, 0);
    DCHECK_GT(tile_w, 0);

    std::unique_ptr<SDL_Surface *[]> numbers(new SDL_Surface *[max_h_tiles]);
    memset(numbers.get(), 0, sizeof(SDL_Surface *) * max_h_tiles);
    SDL_Surface *ruler = nullptr;
    int h = 0;
    for (int i = 0; i < max_h_tiles; ++i) {
        auto txt = Original::sprintf("%d", i);
        numbers[i] = TTF_RenderUTF8_Blended(font_, txt.c_str(), font_color_);
        if (!numbers[i]) {
            LOG(ERROR) << "Can not render TTF." << SDL_GetError();
            goto clean;
        }
        if (numbers[i]->h > h) {
            h = numbers[i]->h;
        }
    }
    ruler = SDL_CreateRGBSurfaceWithFormat(0, tile_w * max_h_tiles, h, 32,
                                           SDL_PIXELFORMAT_RGBA8888);
    if (!ruler) {
        LOG(ERROR) << "Can not create surface." << SDL_GetError();
        goto clean;
    }
    for (int i = 0; i < max_h_tiles; ++i) {
        SDL_Rect dst = {
            i * tile_w + (tile_w - numbers[i]->w) / 2,
            0,
            numbers[i]->w,
            numbers[i]->h,
        };
        SDL_UpperBlit(numbers[i], nullptr, ruler, &dst);
    }
clean:
    for (int i = 0; i < max_h_tiles; ++i) {
        SDL_FreeSurface(numbers[i]);
    }
    return ruler;
}

SDL_Surface *UITerrainView::CreateVRulerSurface(int max_v_tiles,
                                                int tile_h) const {
    DCHECK_GT(max_v_tiles, 0);
    DCHECK_GT(tile_h, 0);

    std::unique_ptr<SDL_Surface *[]> numbers(new SDL_Surface *[max_v_tiles]);
    memset(numbers.get(), 0, sizeof(SDL_Surface *) * max_v_tiles);
    SDL_Surface *ruler = nullptr;
    int w = 0;
    for (int i = 0; i < max_v_tiles; ++i) {
        auto txt = Original::sprintf("%d", i);
        numbers[i] = TTF_RenderUTF8_Blended(font_, txt.c_str(), font_color_);
        if (!numbers[i]) {
            LOG(ERROR) << "Can not render TTF." << SDL_GetError();
            goto clean;
        }
        if (numbers[i]->w > w) {
            w = numbers[i]->w;
        }
    }
    ruler = SDL_CreateRGBSurfaceWithFormat(0, w, max_v_tiles * tile_h, 32,
                                           SDL_PIXELFORMAT_RGBA8888);
    if (!ruler) {
        LOG(ERROR) << "Can not create surface." << SDL_GetError();
        goto clean;
    }
    for (int i = 0; i < max_v_tiles; ++i) {
        SDL_Rect dst = {
            0,
            i * tile_h + (tile_h - numbers[i]->h) / 2,
            numbers[i]->w,
            numbers[i]->h,
        };
        SDL_UpperBlit(numbers[i], nullptr, ruler, &dst);
    }
clean:
    for (int i = 0; i < max_v_tiles; ++i) {
        SDL_FreeSurface(numbers[i]);
    }
    return ruler;
}

int UITerrainView::GetHRulerH() const {
    if (has_ruler_) {
        if (v_ruler_) {
            int w = 0, h = 0;
            SDL_QueryTexture(h_ruler_, nullptr, nullptr, &w, &h);
            return h;
        } else {
            SDL_Surface *surface = nullptr;
            if (max_h_tiles_ == 0 || tile_w_ == 0) {
                surface = CreateHRulerSurface(view_port_h_tiles_, tile_w_);
            } else {
                surface = CreateHRulerSurface(max_h_tiles_, tile_w_);
            }
            return surface ? surface->h : 0;
        }
    }
    return 0;
}

int UITerrainView::GetVRulerW() const {
    if (has_ruler_) {
        if (h_ruler_) {
            int w = 0, h = 0;
            SDL_QueryTexture(v_ruler_, nullptr, nullptr, &w, &h);
            return w;
        } else {
            SDL_Surface *surface = nullptr;
            if (max_v_tiles_ == 0 || tile_h_ == 0) {
                surface = CreateVRulerSurface(view_port_v_tiles_, tile_h_);
            } else {
                surface = CreateVRulerSurface(max_v_tiles_, tile_h_);
            }
            return surface ? surface->w : 0;
        }
    }
    return 0;
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
