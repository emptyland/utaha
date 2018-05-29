#include "fixed-terrain.h"

namespace utaha {

FixedTerrain::FixedTerrain() {
}

FixedTerrain::~FixedTerrain() {
    delete[] tiles_;
    delete[] traps_;
    delete[] entities_;
    delete[] actors_;
}

} // namespace utaha
