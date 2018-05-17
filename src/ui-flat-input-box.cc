#include "ui-flat-input-box.h"
#include "text-input-mode-controller.h"
#include "glog/logging.h"
#include SDL_TTF_H

namespace utaha {

UIFlatInputBox::UIFlatInputBox(TTF_Font *font)
    : font_(font) {
}

/*virtual*/ UIFlatInputBox::~UIFlatInputBox() {
}

/*virtual*/ int UIFlatInputBox::OnEvent(SDL_Event *event, bool *is_break) {
    if (!is_show()) {
        return 0;
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (InRect(rect(), event->button.x, event->button.y)) {
            if (!is_focused()) {
                set_is_focused(true);
                TIMC->OpenTextInputMode();
            }
        } else {
            if (is_focused()) {
                set_is_focused(false);
                TIMC->CloseTextInputMode();
            }
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP) {
        if (InRect(rect(), event->button.x, event->button.y) &&
            is_focused()) {

        } else {
            if (is_focused()) {
                set_is_focused(false);
                TIMC->CloseTextInputMode();
            }
        }
    }

    if (!is_focused()) {
        return 0;
    }
    if (event->type == SDL_TEXTINPUT) {
        //Not copy or pasting
        if(!((event->text.text[ 0 ] == 'c' || event->text.text[ 0 ] == 'C' ) &&
             (event->text.text[ 0 ] == 'v' || event->text.text[ 0 ] == 'V' ) &&
             SDL_GetModState() & KMOD_CTRL)) {
            //Append character
            if (text_w_ < rect().w - padding_size_ * 2 - 4 && text_.length() < max_input_) {
                text_.append(event->text.text);
                is_changed_ = true;
            }
        }
    }
    if (event->type == SDL_KEYDOWN) {
        //Handle backspace
        if(event->key.keysym.sym == SDLK_BACKSPACE && text_.length() > 0) {
            //lop off character
            text_.pop_back();
            is_changed_ = true;
        }
    }
    return 0;
}

/*virtual*/ int UIFlatInputBox::OnRender(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, bg_color_.r, bg_color_.g, bg_color_.b,
                           bg_color_.a);
    SDL_RenderFillRect(renderer, mutable_rect());

    SDL_SetRenderDrawColor(renderer, border_color_.r, border_color_.g,
                           border_color_.b, border_color_.a);
    SDL_RenderDrawRect(renderer, mutable_rect());

    if (!CreateOrSetText(renderer)) {
        return -1;
    }

    if (texture_) {
        const SDL_Rect src{0, 0, text_w_, text_h_};
        const SDL_Rect dst{
            rect().x + padding_size_,
            rect().y + (rect().h - text_h_) / 2,
            text_w_,
            text_h_,
        };
        SDL_RenderCopy(renderer, texture_, &src, &dst);
    }
    if (is_focused() && SDL_GetTicks() % 1000 >= 500) {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

        SDL_Rect cursor_rect {
            rect().x + text_w_ + padding_size_,
            rect().y + padding_size_,
            2,
            rect().h - padding_size_ * 2,
        };
        SDL_RenderDrawRect(renderer, &cursor_rect);
    }
    return 0;
}

bool UIFlatInputBox::CreateOrSetText(SDL_Renderer *renderer) {
    bool should_create = false;
    if (!texture_) {
        if (!text_.empty()) {
            should_create = true;
            is_changed_ = false;
        } else {
            text_w_ = 0;
        }
    } else {
        if (is_changed_) {
            is_changed_ = false;
            if (texture_) {
                SDL_DestroyTexture(texture_);
                texture_ = nullptr;
            }
            should_create = true;
        }
    }
    if (!should_create) {
        return true;
    }
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font_, text_.c_str(), font_color_);
    if (!surface) {
        //LOG(ERROR) << "Render ttf fail!" << SDL_GetError();
        return false;
    }
    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture_) {
        LOG(ERROR) << "Create texture fail!" << SDL_GetError();
        SDL_FreeSurface(surface);
        return false;
    }
    text_w_ = surface->w;
    text_h_ = surface->h;
    SDL_FreeSurface(surface);
    return true;
}

} // namespace utaha
