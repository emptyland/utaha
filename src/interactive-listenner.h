#ifndef UTAHA_INTERACTIVE_LISTENNER_H_
#define UTAHA_INTERACTIVE_LISTENNER_H_

#include "base.h"

namespace utaha {

class UIComponent;

class InteractiveListenner {
public:
    InteractiveListenner() = default;
    virtual ~InteractiveListenner() = default;

    virtual int OnClick(UIComponent *sender, bool *is_break) = 0;

    virtual int OnDoubleClick(UIComponent *sender, bool *is_break) = 0;

    virtual int OnMouseMove(UIComponent *sender, int x, int y,
                            bool *is_break) = 0;

    virtual int OnCommand(UIComponent *sender, int cmd_id, void *param,
                          bool *is_break) = 0;

    virtual int OnEditChanged(UIComponent *sender, bool *is_break) = 0;

    DISALLOW_IMPLICIT_CONSTRUCTORS(InteractiveListenner);
}; // class InteractiveListenner

} // namespace utaha

#endif // UTAHA_INTERACTIVE_LISTENNER_H_
