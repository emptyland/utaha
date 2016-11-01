#ifndef UI_COMMAND_LIST_H_
#define UI_COMMAND_LIST_H_

#include "ui_layout.h"

struct lua_State;

namespace utaha {

class UICommandList : public UILayout {
public:
    UICommandList(int id);
    virtual ~UICommandList();

    static int RegisterTo(lua_State *L);
}; // class UICommandList

} // namespace utaha

#endif // UI_COMMAND_LIST_H_
