#ifndef UTAHA_ANIMATED_SPIRIT_H_
#define UTAHA_ANIMATED_SPIRIT_H_

#include "indexed-spirit.h"
#include "glog/logging.h"

namespace utaha {

template<int N>
class AnimatedSpirit : public IndexedSpirit {
public:
    enum {kNumFrames = N};
    static_assert(N > 0, "Number of frames must great to zero.");

    int frame(int index) const {
        DCHECK_GE(index, 0);
        DCHECK_LT(index, N);
        return frames_[index];
    }

    void set_frame(int index, int tex_id) {
        DCHECK_GE(index, 0);
        DCHECK_LT(index, N);
        frames_[index] = tex_id;
    }

private:
    int frames_[N] = {0};
}; // class AnimatedSpirit

} // namespace utaha

#endif // UTAHA_ANIMATED_SPIRIT_H_
