#ifndef UTAHA_ON_EXIT_SCOPE_H_
#define UTAHA_ON_EXIT_SCOPE_H_

#include "base.h"
#include "glog/logging.h"

namespace utaha {

class OnExitScope {
public:
    typedef void (*OnExitHook)(void *param);

    struct OnExitEntry {
        OnExitEntry *prev;
        OnExitHook   hook;
        void        *param;
    };

    OnExitScope(Initializer);
    ~OnExitScope();

    static OnExitScope *current();

    void HookExit(OnExitHook hook, void *param);

    DISALLOW_IMPLICIT_CONSTRUCTORS(OnExitScope);
private:
    OnExitEntry *head_ = nullptr;
    OnExitScope *prev_ = nullptr;
    static OnExitScope *top_;
}; // class OnExitScope

} // namespace utaha

#endif // UTAHA_ON_EXIT_SCOPE_H_
