#ifndef UTAHA_INDEXED_SPIRIT_H_
#define UTAHA_INDEXED_SPIRIT_H_

#include "base.h"
#include <string>

namespace utaha {

class IndexedSpirit {
public:
    IndexedSpirit() = default;
    ~IndexedSpirit() = default;

    DEF_VAL_PROP_RW(int, id);
    //DEF_VAL_PROP_RW(std::string, name);
    DEF_VAL_PROP_RW(int, w);
    DEF_VAL_PROP_RW(int, h);

    DISALLOW_IMPLICIT_CONSTRUCTORS(IndexedSpirit);
private:
    int id_ = 0;
    //std::string name_;
    int w_ = 0;
    int h_ = 0;
}; // class IndexedSpirit

} // namespace utaha

#endif // UTAHA_INDEXED_SPIRIT_H_
