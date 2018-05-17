#ifndef UTAHA_TEXT_INPUT_MODE_CONTROLLER_H_
#define UTAHA_TEXT_INPUT_MODE_CONTROLLER_H_

#include "lazy-instance.h"
#include "base.h"

namespace utaha {

class TextInputModeController {
public:
    inline TextInputModeController() = default;
    ~TextInputModeController();

    void OpenTextInputMode();
    void CloseTextInputMode();

    bool is_open() const { return demand_ > 0; }

    DISALLOW_IMPLICIT_CONSTRUCTORS(TextInputModeController);
private:
    int demand_ = 0;
}; // class TextInputModeController

extern LazyInstance<TextInputModeController> TIMC;

} // namespace utaha

#endif // UTAHA_TEXT_INPUT_MODE_CONTROLLER_H_
