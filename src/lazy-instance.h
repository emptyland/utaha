#ifndef UTAHA_LAZY_INSTANCE_H_
#define UTAHA_LAZY_INSTANCE_H_

#include "on-exit-scope.h"
#include "base.h"

namespace utaha {

template<class T>
class LazyInstance {
public:
    LazyInstance(Initializer) {}
    ~LazyInstance() {}

    T &get() { return *GetOrCreateInstance(); }
    T *ptr() { return  GetOrCreateInstance(); }

    T *operator -> () { return ptr(); }

private:
    inline T *GetOrCreateInstance();

    static void OnExit(void *param) {
        static_cast<T *>(param)->~T();
    }

    T *core_ = nullptr;
    static char buf_[sizeof(T)];
}; // class LazyInstance

template<class T> /*static*/ char LazyInstance<T>::buf_[sizeof(T)];

template<class T>
inline T *LazyInstance<T>::GetOrCreateInstance() {
    if (core_) {
        return core_;
    }
    core_ = new (buf_) T();
    OnExitScope::current()->HookExit(OnExit, core_);
    return core_;
}

} // namespace utaha

#endif // namespace utaha
