#include "ui_command_list.h"

namespace utaha {

UICommandList::UICommandList(int id)
    : UILayout(id) {
}

UICommandList::~UICommandList() {
}

int UICommandList::RegisterTo(lua_State *L) {
    return 0;
}

} // namespace utaha
