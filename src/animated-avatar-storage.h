#ifndef UTAHA_ANIMATED_AVATAR_STORAGE_H_
#define UTAHA_ANIMATED_AVATAR_STORAGE_H_

#include "generic-storage.h"
#include "animated-avatar.h"

namespace utaha {

class AnimatedAvatarStorage : public GenericStorage<AnimatedAvatar> {
public:
    AnimatedAvatarStorage(int start_id);
    virtual ~AnimatedAvatarStorage();

    virtual bool LoadFromFile(Original *fs) override;
    virtual bool StoreToFile(Original *fs) const override;
    virtual std::string name() const override;

    DISALLOW_IMPLICIT_CONSTRUCTORS(AnimatedAvatarStorage);
}; // class AnimatedAvatarStorage

} // namespace utaha

#endif // UTAHA_ANIMATED_AVATAR_STORAGE_H_
