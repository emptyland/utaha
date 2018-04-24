#include "ui-flat-button.h"
#include "sample-render-entity.h"

namespace utaha {

UIFlatButton::UIFlatButton() {
}

/*virtual*/ UIFlatButton::~UIFlatButton() {
}

/*virtual*/ int UIFlatButton::OnEvent(SDL_Event *event, bool *is_break) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (InRect(rect(), event->button.x, event->button.y)) {
            is_pressed_ = true;
        } else {
            is_pressed_ = false;
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP) {
        is_pressed_ = false;
    }

    return UIComponent::OnEvent(event, is_break);
}

/*virtual*/ int UIFlatButton::OnRender(SDL_Renderer *renderer) {
    if (!is_show()) {
        return 0;
    }

    if (is_pressed_) {
        SDL_SetRenderDrawColor(renderer, pressed_color_.r,
                               pressed_color_.g,
                               pressed_color_.b,
                               pressed_color_.a);
    } else {
        SDL_SetRenderDrawColor(renderer, normal_color_.r,
                               normal_color_.g,
                               normal_color_.b,
                               normal_color_.a);
    }
    SDL_RenderFillRect(renderer, mutable_rect());

    return is_pressed_ ? pressed_->OnRender(renderer) : normal_->OnRender(renderer);
}

bool UIFlatButton::SetNormalText(const char *text, SDL_Color fg, TTF_Font *font,
                                 SDL_Renderer *renderer) {
    normal_.reset();
    std::unique_ptr<SampleTextLabel> label(CreateTextLabel(text, fg, font, renderer));
    normal_.swap(label);
    return normal_.get() != nullptr;
}

bool UIFlatButton::SetPressedText(const char *text, SDL_Color fg, TTF_Font *font,
                                  SDL_Renderer *renderer) {
    pressed_.reset();
    std::unique_ptr<SampleTextLabel> label(CreateTextLabel(text, fg, font, renderer));
    pressed_.swap(label);
    return pressed_.get() != nullptr;
}

SampleTextLabel *
UIFlatButton::CreateTextLabel(const char *text, SDL_Color fg, TTF_Font *font,
                              SDL_Renderer *renderer) {
    std::unique_ptr<SampleTextLabel> label(new SampleTextLabel(font));
    if (!label->SetTextBlended(text, fg, renderer)) {
        return nullptr;
    }

    int dw = (rect().w - label->surface()->w) / 2;
    int dh = (rect().h - label->surface()->h) / 2;

    label->set_rect({
        .x = rect().x + dw,
        .y = rect().y + dh,
        .w = label->surface()->w,
        .h = label->surface()->h,
    });
    return label.release();
}

} // namespace utaha
