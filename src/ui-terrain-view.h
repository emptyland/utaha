#ifndef UTAHA_UI_TERRAIN_VIEW_H_
#define UTAHA_UI_TERRAIN_VIEW_H_

#include "fixed-terrain.h"
#include "ui-component.h"
#include "glog/logging.h"
#include <vector>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class IndexedTile;
class GridPicStorage;

template<class T, int N=10> class GenericStorage;

class UITerrainView : public UIComponent {
public:
    UITerrainView(TTF_Font *font);
    virtual ~UITerrainView();

    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RMW(SDL_Color, grid_color);
    DEF_VAL_PROP_RW(int, padding_size);
    DEF_PTR_PROP_RW_NOTNULL1(const GenericStorage<IndexedTile>, tiles);
    DEF_PTR_PROP_RW_NOTNULL1(const GridPicStorage, tile_tex);
    DEF_VAL_PROP_RW(int, cmd_id);

    DEF_VAL_PROP_RW(int, scrolling_speed);

    DEF_VAL_PROP_RW(int, tile_w);
    DEF_VAL_PROP_RW(int, tile_h);
    DEF_VAL_PROP_RW(int, max_h_tiles);
    DEF_VAL_PROP_RW(int, max_v_tiles);
    DEF_PTR_PROP_RW_NOTNULL1(const std::vector<int>, terrain_tiles);
    DEF_PTR_PROP_RW_NOTNULL1(const std::vector<IndexedLinker>, terrain_linkers);

    inline SDL_Rect view_rect() const;

    virtual int OnEvent(SDL_Event *event, bool *is_break) override;
    virtual int OnRender(SDL_Renderer *renderer) override;
    virtual void UpdateRect() override;

    bool InvalidateWhole();

    DISALLOW_IMPLICIT_CONSTRUCTORS(UITerrainView);
private:
    bool CreateSurface();
    SDL_Surface *CreateOrGetMissionSurface();

    SDL_Surface *kMiss = nullptr;

    TTF_Font *font_;
    SDL_Color font_color_ = {0, 0, 0, 0};
    SDL_Color border_color_ = {0, 0, 0, 0};
    SDL_Color grid_color_ = {0, 0, 0, 0};
    int padding_size_ = 10;
    SDL_Surface *whole_ = nullptr;
    SDL_Texture *texture_ = nullptr;
    int cmd_id_ = 0;

    int view_port_x_ = 0;
    int view_port_y_ = 0;
    int scrolling_speed_ = 17;

    const GenericStorage<IndexedTile> *tiles_ = nullptr;
    const GridPicStorage *tile_tex_ = nullptr;
    int tile_w_ = 0;
    int tile_h_ = 0;
    int max_h_tiles_ = 0;
    int max_v_tiles_ = 0;
    const std::vector<int> *terrain_tiles_;
    const std::vector<IndexedLinker> *terrain_linkers_;
}; // class UITerrainView

inline SDL_Rect UITerrainView::view_rect() const {
    return {
        rect().x + padding_size_,
        rect().y + padding_size_,
        rect().w - padding_size_ * 2,
        rect().h - padding_size_ * 2,
    };
}

} // namespace utaha

#endif //UTAHA_UI_TERRAIN_VIEW_H_
