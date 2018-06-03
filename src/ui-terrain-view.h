#ifndef UTAHA_UI_TERRAIN_VIEW_H_
#define UTAHA_UI_TERRAIN_VIEW_H_

#include "fixed-terrain.h"
#include "ui-component.h"
#include "glog/logging.h"
#include <vector>
#include <memory>

typedef struct _TTF_Font TTF_Font;

namespace utaha {

class IndexedTile;
class GridPicStorage;

template<class T, int N=10> class GenericStorage;

struct TerrainViewEvent {
    enum Event {
        LINKER_SELECTED,
        ENTITY_SELECTED,
        ACTOR_SELECTED,
        TILE_SELECTED,
        MOUSE_MOVED,
    };
    Event event;
    union {
        struct {
            int x;
            int y;
            int id;
        } tile;
        struct {
            int x;
            int y;
        } mouse;
    };
};

class UITerrainView : public UIComponent {
public:
    enum Mode {
        SELECT_MODE,
        PLACE_MODE,
    };

    UITerrainView(TTF_Font *font);
    virtual ~UITerrainView();

    DEF_VAL_PROP_RMW(SDL_Color, font_color);
    DEF_VAL_PROP_RMW(SDL_Color, border_color);
    DEF_VAL_PROP_RMW(SDL_Color, grid_color);
    DEF_VAL_PROP_RW(int, padding_size);
    DEF_PTR_PROP_RW_NOTNULL1(const GenericStorage<IndexedTile>, tiles);
    DEF_PTR_PROP_RW_NOTNULL1(const GridPicStorage, tile_tex);
    DEF_VAL_PROP_RW(int, cmd_id);

    DEF_VAL_PROP_RW(bool, has_ruler);
    DEF_VAL_PROP_RW(int, scrolling_speed);
    DEF_VAL_PROP_RW(int, view_port_h_tiles);
    DEF_VAL_PROP_RW(int, view_port_v_tiles);

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
    int RenderRuler(int bx, int by, int vx, int vy, int vw, int vh,
                    SDL_Renderer *renderer);
    SDL_Texture *CreateOrGetMissionGrid(SDL_Renderer *renderer);
    int ShiftRuler(std::vector<SDL_Texture *> *ruler, int n);
    int GetHRulerH() const;
    int GetVRulerW() const;
    int ProcessTileSelected(SDL_Event *e, bool *is_break);

    SDL_Texture *kMiss = nullptr;
    mutable int kMaxHRulerH = 0;
    mutable int kMaxVRulerW = 0;

    TTF_Font *font_;
    SDL_Color font_color_ = {0, 0, 0, 0};
    SDL_Color border_color_ = {0, 0, 0, 0};
    SDL_Color grid_color_ = {0, 0, 0, 0};
    int padding_size_ = 10;
    std::vector<SDL_Texture *> h_ruler_;
    std::vector<SDL_Texture *> v_ruler_;
    std::vector<SDL_Texture *> indexed_tex_;
    int cmd_id_ = 0;
    Mode mode_ = PLACE_MODE;
    bool has_ruler_ = false;
    int view_port_x_ = 0;
    int view_port_y_ = 0;
    int view_port_h_tiles_ = 16;
    int view_port_v_tiles_ = 9;
    int scrolling_speed_ = 1;

    const GenericStorage<IndexedTile> *tiles_ = nullptr;
    const GridPicStorage *tile_tex_ = nullptr;
    int tile_w_ = 0;
    int tile_h_ = 0;
    int max_h_tiles_ = 0;
    int max_v_tiles_ = 0;
    const std::vector<int> *terrain_tiles_ = nullptr;
    const std::vector<IndexedLinker> *terrain_linkers_ = nullptr;
}; // class UITerrainView

inline SDL_Rect UITerrainView::view_rect() const {
    return { // 垂直和水平标尺一样宽
        rect().x + padding_size_ + GetVRulerW(),
        rect().y + padding_size_ + GetHRulerH(),
        rect().w - padding_size_ * 2 - GetVRulerW(),
        rect().h - padding_size_ * 2 - GetHRulerH(),
    };
}

} // namespace utaha

#endif //UTAHA_UI_TERRAIN_VIEW_H_
