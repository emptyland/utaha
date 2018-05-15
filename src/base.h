#ifndef UTAHA_BASE_H_
#define UTAHA_BASE_H_

#include <stddef.h>

namespace utaha {

#define DISALLOW_IMPLICIT_CONSTRUCTORS(clazz_name) \
    clazz_name (const clazz_name &) = delete;      \
    clazz_name (clazz_name &&) = delete;           \
    void operator = (const clazz_name &) = delete;

/**
 * disable copy constructor, assign operator function.
 *
 */
class DisallowImplicitConstructors {
public:
    DisallowImplicitConstructors() = default;

    DISALLOW_IMPLICIT_CONSTRUCTORS(DisallowImplicitConstructors)
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
#define DEF_VAL_PROP_RMW(type, name) \
    DEF_VAL_GETTER(type, name) \
    DEF_VAL_MUTABLE_GETTER(type, name) \
    DEF_VAL_SETTER(type, name)

#define DEF_VAL_PROP_RW(type, name) \
    DEF_VAL_GETTER(type, name) \
    DEF_VAL_SETTER(type, name)

#define DEF_PTR_PROP_RW(type, name) \
    DEF_PTR_GETTER(type, name) \
    DEF_PTR_SETTER(type, name)

#define DEF_PTR_PROP_RW_NOTNULL1(type, name) \
    DEF_PTR_GETTER(type, name) \
    DEF_PTR_SETTER_NOTNULL(type, name)

#define DEF_PTR_PROP_RW_NOTNULL2(type, name) \
    DEF_PTR_GETTER_NOTNULL(type, name) \
    DEF_PTR_SETTER_NOTNULL(type, name)

#define DEF_VAL_GETTER(type, name) \
    inline const type &name() const { return name##_; }

#define DEF_VAL_MUTABLE_GETTER(type, name) \
    inline type *mutable_##name() { return &name##_; }

#define DEF_VAL_SETTER(type, name) \
    inline void set_##name(const type &value) { name##_ = value; }

#define DEF_PTR_GETTER(type, name) \
    inline type *name() const { return name##_; }

#define DEF_PTR_SETTER(type, name) \
    inline void set_##name(type *value) { name##_ = value; }

#define DEF_PTR_GETTER_NOTNULL(type, name) \
    inline type *name() const { return DCHECK_NOTNULL(name##_); }

#define DEF_PTR_SETTER_NOTNULL(type, name) \
    inline void set_##name(type *value) { name##_ = DCHECK_NOTNULL(value); }

#define COLOR_R_MASK 0x000000ff
#define COLOR_G_MASK 0x0000ff00
#define COLOR_B_MASK 0x00ff0000
#define COLOR_A_MASK 0xff000000

// OS macro checking:
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#define UTAHA_OS_WINDOWS 1
#endif

#if defined(unix) || defined(__unix) || defined(__unix__)
#define UTAHA_OS_UNIX 1
#endif

#if defined(linux) || defined(__linux) defined(__linux__) defined(__gnu_linux__)
#define UTAHA_OS_LINUX 1
#endif

#if defined(__APPLE__)
#define UTAHA_OS_MACOS 1
#endif


#if defined(UTAHA_OS_UNIX) || defined(UTAHA_OS_LINUX) || defined(UTAHA_OS_MACOS)
#define SDL_H       <SDL2/SDL.h>
#define SDL_IMAGE_H <SDL2_image/SDL_image.h>
#define SDL_TTF_H   <SDL2_ttf/SDL_ttf.h>
#endif

#if defined(UTAHA_OS_WINDOWS)
#define SDL_H       "SDL.h"
#define SDL_IMAGE_H "SDL_image.h"
#define SDL_TTF_H   "SDL_ttf.h"
#endif 

} // namespace utaha

#endif // UTAHA_BASE_H_
