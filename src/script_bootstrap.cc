#include "ui_command_list.h"
#include "lua_utils.h"
#include "glog/logging.h"

namespace utaha {

struct RegisterEntry {
    const char *name;
    int (*lua_callback)(lua_State *L);
};

RegisterEntry boot_entries[] = {

    // UI part

    { "ui.CommandList", &UICommandList::RegisterTo },

    // EOF
    { .name = nullptr, .lua_callback = nullptr },
};


int ScriptBoot(lua_State *L) {
    RegisterEntry *entry = nullptr;
    int i = 0;
    do {
        entry = &boot_entries[i++];
        if (entry->name) {
            DLOG(INFO) << entry->name << " register to lua.";
        }
    } while (entry->name && entry->lua_callback);

    return i;
}

} // namespace utaha
