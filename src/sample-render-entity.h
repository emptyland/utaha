#ifndef UTAHA_SAMPLE_RENDER_ENTITY_H_
#define UTAHA_SAMPLE_RENDER_ENTITY_H_

#include "render-entity.h"
#include "glog/logging.h"
#include <SDL2_ttf/SDL_ttf.h>

namespace utaha {

class SampleTextLabel : public RenderEntity {
public:
    SampleTextLabel(TTF_Font *font);
    virtual ~SampleTextLabel();
    virtual int OnRender(SDL_Renderer *renderer) override;

    DEF_PTR_GETTER_NOTNULL(SDL_Surface, surface);
    DEF_VAL_PROP_RW(SDL_Rect, rect);

    bool SetTextSolid(const char *text, SDL_Color color, SDL_Renderer *renderer);
    bool SetTextShaded(const char *text, SDL_Color fg, SDL_Color bg, SDL_Renderer *renderer);
    bool SetTextBlended(const char *text, SDL_Color fg, SDL_Renderer *renderer);
    void Destory();

    DISALLOW_IMPLICIT_CONSTRUCTORS(SampleTextLabel);
private:
    bool CreateTexture(SDL_Renderer *renderer);

    SDL_Surface *surface_ = nullptr;
    SDL_Texture *texture_ = nullptr;
    TTF_Font *font_;
    SDL_Rect rect_;
};

} // namespace utaha

#endif // UTAHA_SAMPLE_RENDER_ENTITY_H_
