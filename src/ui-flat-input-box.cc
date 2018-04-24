#include "ui-flat-input-box.h"
#include "sample-render-entity.h"

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

    int result = UIComponent::OnEvent(event, is_break);
    if (*is_break) {
        return result;
    }
    if (is_focused()) {
        SDL_StartTextInput();
    } else {
        SDL_StopTextInput();
    }

    if (event->type == SDL_TEXTINPUT) {
        //Not copy or pasting
        if(!((event->text.text[ 0 ] == 'c' || event->text.text[ 0 ] == 'C' ) &&
             (event->text.text[ 0 ] == 'v' || event->text.text[ 0 ] == 'V' ) &&
             SDL_GetModState() & KMOD_CTRL)) {
            //Append character
            if (text_.length() < max_input_) {
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

    SDL_Rect cursor_rect;
    if (label_) {
        label_->OnRender(renderer);
        cursor_rect = {
            .x = label_->rect().x + label_->rect().w + padding_size_,
            .y = label_->rect().y,
            .w = 2,
            .h = label_->rect().h,
        };
    } else {
        cursor_rect = {
            .x = rect().x + padding_size_,
            .y = rect().y + padding_size_,
            .w = 2,
            .h = rect().h - padding_size_ * 2,
        };
    }
    if (is_focused() && SDL_GetTicks() % 1000 >= 500) {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
        SDL_RenderDrawRect(renderer, &cursor_rect);
    }
    return 0;
}

bool UIFlatInputBox::CreateOrSetText(SDL_Renderer *renderer) {
    if (!label_) {
        if (!text_.empty()) {
            std::unique_ptr<SampleTextLabel> label(new SampleTextLabel(font_));
            if (!label->SetTextBlended(text_.c_str(), text_color_, renderer)) {
                return false;
            }
            label_.swap(label);
            is_changed_ = false;
        }
    } else {
        if (is_changed_) {
            is_changed_ = false;

            if (text_.empty()) {
                label_.reset();
            } else if (!label_->SetTextBlended(text_.c_str(), text_color_, renderer)) {
                return false;
            }
        }
    }

    if (!label_) {
        return true;
    }
    int dh = (rect().h - label_->surface()->h) / 2;
    label_->set_rect({
        .x = rect().x + padding_size_,
        .y = rect().y + dh,
        .w = std::min(label_->surface()->w, rect().w - padding_size_),
        .h = label_->surface()->h,
    });
    return true;
}

} // namespace utaha
