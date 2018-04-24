#include "sample-render-entity.h"
#include "glog/logging.h"

namespace utaha {

SampleTextLabel::SampleTextLabel(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font))
    , rect_({0, 0, 0, 0}) {
}

/*virtual*/ SampleTextLabel::~SampleTextLabel() {
    Destory();
}

/*virtual*/ int SampleTextLabel::OnRender(SDL_Renderer *renderer) {
    return SDL_RenderCopy(renderer, texture_, nullptr, &rect_);
}

bool SampleTextLabel::SetTextSolid(const char *text, SDL_Color color,
                                   SDL_Renderer *renderer) {
    Destory();
    surface_ = TTF_RenderUTF8_Solid(font_, text, color);
    if (!surface_) {
        return false;
    }
    return CreateTexture(renderer);
}

bool SampleTextLabel::SetTextShaded(const char *text, SDL_Color fg, SDL_Color bg,
                                    SDL_Renderer *renderer) {
    Destory();
    surface_ = TTF_RenderUTF8_Shaded(font_, text, fg, bg);
    if (!surface_) {
        return false;
    }
    return CreateTexture(renderer);
}

bool SampleTextLabel::SetTextBlended(const char *text, SDL_Color fg, SDL_Renderer *renderer) {
    Destory();
    surface_ = TTF_RenderUTF8_Blended(font_, text, fg);
    if (!surface_) {
        return false;
    }
    return CreateTexture(renderer);
}

void SampleTextLabel::Destory() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
    if (surface_) {
        SDL_FreeSurface(surface_);
        surface_ = nullptr;
    }
}

bool SampleTextLabel::CreateTexture(SDL_Renderer *renderer) {
    texture_ = SDL_CreateTextureFromSurface(renderer, surface_);
    if (!texture_) {
        DLOG(ERROR) << "Can not create texture: " << SDL_GetError();
        return false;
    }
    return true;
}

} // namespace utaha

