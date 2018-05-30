#include "ui-terrain-view.h"
#include "grid-pic-storage.h"
#include "generic-storage.h"
#include "indexed-tile.h"
#include "fixed-terrain.h"

namespace utaha {

UITerrainView::UITerrainView(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
}

/*virtual*/ UITerrainView::~UITerrainView() {
    if (whole_) {
        SDL_FreeSurface(whole_);
        whole_ = nullptr;
    }
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

/*virtual*/ int UITerrainView::OnEvent(SDL_Event *event, bool *is_break) {
    if (!is_show() || !whole_) {
        return 0;
    }

    int result = UIComponent::OnEvent(event, is_break);
    if (*is_break) {
        return result;
    }

    if (!focused()) {
        return 0;
    }

    SDL_Rect view_rc = view_rect();
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                if (view_port_y_ >= 0) {
                    view_port_y_ -= scrolling_speed_;
                }
                if (view_port_y_ < 0) {
                    view_port_y_ = 0;
                }
                break;
            case SDLK_DOWN:
            case SDLK_s:
                if (view_port_y_ >= 0) {
                    view_port_y_ += scrolling_speed_;
                }
                if (view_port_y_ + view_rc.h > tile_h_ * max_v_tiles_) {
                    view_port_y_ = tile_h_ * max_v_tiles_ - view_rc.h;
                }
                break;
            case SDLK_LEFT:
            case SDLK_a:
                if (view_port_x_ >= 0) {
                    view_port_x_ -= scrolling_speed_;
                }
                if (view_port_x_ < 0) {
                    view_port_x_ = 0;
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                if (view_port_x_ >= 0) {
                    view_port_x_ += scrolling_speed_;
                }
                if (view_port_x_ + view_rc.w > tile_w_ * max_h_tiles_) {
                    view_port_x_ = tile_w_ * max_h_tiles_ - view_rc.w;
                }
                break;
        }
    }
    return 0;
}

/*virtual*/ int UITerrainView::OnRender(SDL_Renderer *renderer) {
    if (!is_show() || !whole_) {
        return 0;
    }

    if (!texture_) {
        texture_ = SDL_CreateTextureFromSurface(renderer, whole_);
        if (!texture_) {
            LOG(ERROR) << "Can not create texture. " << SDL_GetError();
            return -1;
        }
    }

    const SDL_Rect view_rc = view_rect();
    SDL_Rect src, dst;
    if (view_port_x_ >= 0) {
        src.x = 0 + view_port_x_;
        src.w = view_rc.w;
        dst.x = view_rc.x;
        dst.w = view_rc.w;
    } else {
        src.x = 0;
        src.w = tile_w_ * max_h_tiles_;
        dst.x = view_rc.x - view_port_x_;
        dst.w = tile_w_ * max_h_tiles_;
    }
    if (view_port_y_ >= 0) {
        src.y = 0 + view_port_y_;
        src.h = view_rc.h;
        dst.y = view_rc.y;
        dst.h = view_rc.h;
    } else {
        src.y = 0;
        src.h = tile_h_ * max_v_tiles_;
        dst.y = view_rc.y - view_port_y_;
        dst.h = tile_h_ * max_v_tiles_;
    }
    SDL_RenderCopy(renderer, texture_, &src, &dst);

    SDL_SetRenderDrawColor(renderer, border_color_.r, border_color_.g,
                           border_color_.b, border_color_.a);
    SDL_RenderDrawRect(renderer, &rect());

    int x, y;
    if (view_port_x_ >= 0) {
        x = dst.x + (tile_w_ - view_port_x_ % tile_w_);
    } else {
        x = dst.x;
    }
    if (view_port_y_ >= 0) {
        y = dst.y + (tile_h_ - view_port_y_ % tile_h_);
    } else {
        y = dst.y;
    }
    SDL_SetRenderDrawColor(renderer, grid_color_.r, grid_color_.g,
                           grid_color_.b, grid_color_.a);
    for (int i = x; i < dst.x + dst.w; i += tile_w_) {
        SDL_RenderDrawLine(renderer, i, dst.y, i, dst.y + dst.h);
    }
    for (int i = y; i < dst.y + dst.h; i += tile_h_) {
        SDL_RenderDrawLine(renderer, dst.x, i, dst.x + dst.w, i);
    }
    return 0;
}

/*virtual*/ void UITerrainView::UpdateRect() {
}

bool UITerrainView::InvalidateWhole() {
    if (whole_) {
        SDL_FreeSurface(whole_);
        whole_ = nullptr;
    }
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }

    auto view_rc = view_rect();
    view_port_x_ = (tile_w_ * max_h_tiles_ - view_rc.w) / 2;
    view_port_y_ = (tile_h_ * max_v_tiles_ - view_rc.h) / 2;

    return CreateSurface();
}

bool UITerrainView::CreateSurface() {
    DCHECK_GT(tile_w_, 0);
    DCHECK_GT(tile_h_, 0);
    DCHECK_GT(max_h_tiles_, 0);
    DCHECK_GT(max_v_tiles_, 0);
    whole_ = SDL_CreateRGBSurfaceWithFormat(0, max_h_tiles_ * tile_w_,
                                            max_v_tiles_ * tile_h_, 32,
                                            SDL_PIXELFORMAT_RGBA8888);
    if (!whole_) {
        LOG(ERROR) << "Can not create surface. " << SDL_GetError();
        return false;
    }

    DCHECK_NOTNULL(terrain_tiles_);
    DCHECK_EQ(max_h_tiles_ * max_v_tiles_, terrain_tiles_->size());
    for (int y = 0; y < max_v_tiles_; ++y) {
        for (int x = 0; x < max_h_tiles_; ++x) {
            int tile_id = (*terrain_tiles_)[y * max_h_tiles_ + x];
            if (tile_id == 0) {
                continue;
            }

            SDL_Surface *grid = nullptr;
            const IndexedTile *tile = tiles_->FindOrNull(tile_id);
            if (!tile) {
                LOG(WARNING) << "Tile id: " << tile_id << " not found!";
                grid = CreateOrGetMissionSurface();
            } else {
                grid = tile_tex_->FindOrNullGrid(tile->tex_id());
                if (!grid) {
                    LOG(WARNING) << "Tile tex_id: " << tile->tex_id()
                                 << " not found!";
                    grid = CreateOrGetMissionSurface();
                }
            }
            SDL_Rect dst = {
                x * tile_w_,
                y * tile_h_,
                tile_w_,
                tile_h_,
            };
            SDL_Rect src = {0, 0, grid->w, grid->h};
            SDL_UpperBlitScaled(grid, &src, whole_, &dst);
        }
    }
    return true;
}

SDL_Surface *UITerrainView::CreateOrGetMissionSurface() {
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
    kMiss = miss;
    return kMiss;
}

} // namespace utaha
