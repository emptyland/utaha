#ifndef UTAHA_CHARACTER_H_
#define UTAHA_CHARACTER_H_

#include "datatype.h"
#include "base.h"
#include <stdarg.h>

namespace utaha {

class Character8 : public DisableCopyAndAssign {
public:
    Character8();
    virtual ~Character8();

    virtual void Vprintf(const char *fmt, va_list ap) = 0;

    inline void Printf(const char *fmt, ...);

    DEF_PROP_RMW(Float3, position)
    DEF_PROP_RMW(Float3, color)
    DEF_PROP_RMW(Float2, size)
private:
    Float3 position_;
    Float3 color_;
    Float2 size_;
}; // Character

inline void Character8::Printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    Vprintf(fmt, ap);
    va_end(ap);
}

Character8 *CreateASCIICharacter(int tex_id);

} // namespace utaha

#endif // UTAHA_CHARACTER_H_