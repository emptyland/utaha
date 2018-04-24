#ifndef UTAHA_INTERACTIVE_ENTITY_H_
#define UTAHA_INTERACTIVE_ENTITY_H_

#include "render-entity.h"

namespace utaha {

class InteractiveListenner;

class InteractiveEntity : public RenderEntity {
public:
    InteractiveEntity();
    virtual ~InteractiveEntity();

    virtual int OnEvent(SDL_Event *event, bool *is_break);

    void AddListenner(InteractiveListenner *listenner) {
        listenner_ = listenner;
    }

protected:
    InteractiveListenner *listenner_ = nullptr;
}; // class InteractiveEntity

} // namespace utaha

#endif // UTAHA_INTERACTIVE_ENTITY_H_
