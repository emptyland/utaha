#ifndef UTAHA_ANIMATED_AVATAR_H_
#define UTAHA_ANIMATED_AVATAR_H_

#include "animated-spirit.h"

namespace utaha {

enum Direction: int {
    DIR_N, // UP
    DIR_S, // Down
    DIR_W, // Left
    DIR_E, // Right
    MAX_DIR,
};

class AnimatedAvatar {
public:
    typedef AnimatedSpirit<3> Animation;

    inline AnimatedAvatar() = default;
    inline ~AnimatedAvatar() = default;

    DEF_VAL_PROP_RW(int, id);
    DEF_VAL_PROP_RW(std::string, name);
    DEF_VAL_PROP_RW(int, w);
    DEF_VAL_PROP_RW(int, h);

    const Animation &animation(Direction dir) const {
        return animation_[dir];
    }

    int frame(Direction dir, int index) const {
        return animation(dir).frame(index);
    }

    Animation *mutable_animation(Direction dir) {
        return &animation_[dir];
    }

    void set_frame(Direction dir, int index, int tex_id) {
        animation_[dir].set_frame(index, tex_id);
    }

    void CopyTo(AnimatedAvatar *other) const;

    DISALLOW_IMPLICIT_CONSTRUCTORS(AnimatedAvatar);
private:
    int id_;
    std::string name_;
    Animation animation_[MAX_DIR];
    int w_;
    int h_;
}; // class AnimatedAvatar

} // namespace utaha

#endif // UTAHA_ANIMATED_AVATAR_H_
