#include "ui-flat-text-box.h"
#include "base-io.h"
#include "glog/logging.h"
#include SDL_TTF_H
#include <stdarg.h>
#include <algorithm>

namespace utaha {

UIFlatTextBox::UIFlatTextBox(TTF_Font *font)
    : font_(DCHECK_NOTNULL(font)) {
}

/*virtual*/ UIFlatTextBox::~UIFlatTextBox() {
    for (auto line : lines_) {
        if (std::get<1>(line)) {
            SDL_DestroyTexture(std::get<1>(line));
        }
    }
    lines_.clear();
}

/*virtual*/ int UIFlatTextBox::OnRender(SDL_Renderer *renderer) {
    if (!is_show()) {
        return 0;
    }
    std::string text;
    for (size_t i = 0; i < lines_.size(); ++i) {
        SDL_Texture *tex;
        std::tie(text, tex) = lines_[i];
        if (tex) {
            continue;
        }
        SDL_Surface *surface =
            TTF_RenderText_Blended(font_, text.empty() ? " " : text.c_str(),
                                   font_color_);
        if (!surface) {
            LOG(ERROR) << "Can not reader text. " << SDL_GetError();
            return -1;
        }
        tex = SDL_CreateTextureFromSurface(renderer, surface);
        if (!tex) {
            LOG(ERROR) << "Can not create texure. " << SDL_GetError();
            SDL_FreeSurface(surface);
            return -1;
        }
        SDL_FreeSurface(surface);
        std::get<1>(lines_[i]) = tex;
    }

    int y = rect().y + padding_size_;
    for (size_t i = 0; i < lines_.size(); ++i) {
        SDL_Texture *tex = std::get<1>(lines_[i]);

        int h, w;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        SDL_Rect dst = {
            rect().x + padding_size_,
            y,
            std::min(w, rect().w - 2 * padding_size_),
            h,
        };
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
        y += (padding_size_ + h);
    }

    SDL_SetRenderDrawColor(renderer, border_color_.r, border_color_.g,
                           border_color_.b, border_color_.a);
    SDL_RenderDrawRect(renderer, &rect());
    return 0;
}

void UIFlatTextBox::SetLinesSize(int size) {
    for (auto line : lines_) {
        if (std::get<1>(line)) {
            SDL_DestroyTexture(std::get<1>(line));
        }
    }
    lines_.clear();
    lines_.resize(size, std::make_tuple("", nullptr));
}

void UIFlatTextBox::SetLine(int i, const std::string &text) {
    DCHECK_GE(i, 0);
    DCHECK_LT(i, lines_.size());

    if (std::get<1>(lines_[i])) {
        SDL_DestroyTexture(std::get<1>(lines_[i]));
    }
    std::get<0>(lines_[i]) = text;
}

void UIFlatTextBox::SetFormatLine(int i, const char *fmt, ...) {
    DCHECK_GE(i, 0);
    DCHECK_LT(i, lines_.size());

    va_list ap;
    va_start(ap, fmt);
    SetLine(i, Original::vsprintf(fmt, ap));
    va_end(ap);
}

} // namespace utaha
