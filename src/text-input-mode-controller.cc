#include "text-input-mode-controller.h"
#include "glog/logging.h"
#include SDL_H

namespace utaha {

LazyInstance<TextInputModeController> TIMC(LAZY_INSTANCE_INITIALIZER);

TextInputModeController::~TextInputModeController() {
    if (demand_ > 0) {
        SDL_StopTextInput();
    }
}

void TextInputModeController::OpenTextInputMode() {
    if (++demand_ == 1) {
        SDL_StartTextInput();
        DLOG(INFO) << "Start text input!";
    }
}

void TextInputModeController::CloseTextInputMode() {
    if (demand_ == 0) {
        return;
    }
    if (--demand_ == 0) {
        SDL_StopTextInput();
        DLOG(INFO) << "Stop text input!";
    }
}

} // namespace utaha
