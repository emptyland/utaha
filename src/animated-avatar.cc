#include "animated-avatar.h"

namespace utaha {

void AnimatedAvatar::CopyTo(AnimatedAvatar *other) const {
    other->id_ = id_;
    other->name_ = name_;
    other->w_ = w_;
    other->h_ = h_;
    for (int i = 0; i < MAX_DIR; ++i) {
        auto d = static_cast<Direction>(i);
        for (int j = 0; j < Animation::kNumFrames; ++j) {
            other->set_frame(d, j, frame(d, j));
        }
    }
}

} // namespace utaha
