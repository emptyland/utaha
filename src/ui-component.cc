#include "ui-component.h"
#include "interactive-listenner.h"

namespace utaha {

UIComponent::UIComponent()
    : rect_({0, 0, 0, 0}) {
}

/*virtual*/ UIComponent::~UIComponent(){
}

/*virtual*/int UIComponent::OnEvent(SDL_Event *event, bool *is_break) {
    if (event->type == SDL_MOUSEMOTION) {
        if(InRect(rect_, event->motion.x, event->motion.y)) {
            if (listenner_) {
                listenner_->OnMouseMove(this, event->motion.x, event->motion.y,
                                        is_break);
            }
        }
    }
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (InRect(rect_, event->button.x, event->button.y)) {
            is_focused_ = true;
        } else {
            is_focused_ = false;
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP) {
        if (InRect(rect_, event->button.x, event->button.y) &&
            is_focused_) {
            if (listenner_) {
                if (event->button.clicks == 0x1) {
                    listenner_->OnClick(this, is_break);
                } else if (event->button.clicks == 0x2) {
                    listenner_->OnDoubleClick(this, is_break);
                }
            }
        } else {
            is_focused_ = false;
        }
    }

    return InteractiveEntity::OnEvent(event, is_break);
}

} // namespace utaha
