#ifndef UTAHA_BASE_H_
#define UTAHA_BASE_H_

#include <stddef.h>

namespace utaha {

/**
 * disable copy constructor, assign operator function.
 *
 */
class DisableCopyAndAssign {
public:
    DisableCopyAndAssign() = default;

    DisableCopyAndAssign(const DisableCopyAndAssign &) = delete;
    DisableCopyAndAssign(DisableCopyAndAssign &&) = delete;
    void operator = (const DisableCopyAndAssign &) = delete;
};

/**
 * Get size of array.
 */
template <class T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#ifndef _MSC_VER
template <class T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(::utaha::ArraySizeHelper(array)))

/**
 * define getter/mutable_getter/setter
 */
#define DEF_PROP_RMW(type, name) \
    DEF_GETTER(type, name) \
    DEF_MUTABLE_GETTER(type, name) \
    DEF_SETTER(type, name)

#define DEF_PROP_RW(type, name) \
    DEF_GETTER(type, name) \
    DEF_SETTER(type, name)

#define DEF_GETTER(type, name) \
    inline const type &name() const { return name##_; }

#define DEF_MUTABLE_GETTER(type, name) \
    inline type *mutable_##name() { return &name##_; }

#define DEF_SETTER(type, name) \
    inline void set_##name(const type &value) { name##_ = value; }

} // namespace utaha

#endif // UTAHA_BASE_H_