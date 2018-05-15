#include "on-exit-scope.h"

namespace utaha {

/*static*/ OnExitScope *OnExitScope::top_ = nullptr;

OnExitScope::OnExitScope(Initializer) {
    prev_ = top_;
    DCHECK_NE(top_, this);
    top_ = this;
}

OnExitScope::~OnExitScope() {
    while (head_) {
        head_->hook(head_->param);
        auto prev = head_->prev;
        delete head_;
        head_ = prev;
    }

    DCHECK_EQ(top_, this);
    top_ = prev_;
}

void OnExitScope::HookExit(OnExitHook hook, void *param) {
    auto node = new OnExitEntry{head_, DCHECK_NOTNULL(hook), param};
    head_ = node;
}

/*static*/ OnExitScope *OnExitScope::current() {
    if (top_ == nullptr) {
        LOG(ERROR) << "OnExitScope has not initialized!";
    }
    return top_;
}

} // namespace utaha
