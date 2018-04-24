#include "interactive-entity.h"
#include "interactive-listenner.h"

namespace utaha {

InteractiveEntity::InteractiveEntity() {
}

/*virtual*/ InteractiveEntity::~InteractiveEntity() {
}

/*virtual*/ int InteractiveEntity::OnEvent(SDL_Event */*event*/, bool */*is_break*/) {
    return 0;
}

} // namespace utaha
