#ifndef UTAHA_ANIMATED_AVATAR_STORAGE_H_
#define UTAHA_ANIMATED_AVATAR_STORAGE_H_

#include "base.h"
#include <map>
#include <vector>
#include <string>

namespace utaha {

class AnimatedAvatar;

class AnimatedAvatarStorage {
public:
    AnimatedAvatarStorage(int start_id)
        : start_id_(start_id), next_id_(start_id) {}
    ~AnimatedAvatarStorage();

    DEF_VAL_PROP_RW(std::string, dir);
    DEF_VAL_PROP_RW(std::string, grid_pic_name);
    DEF_VAL_GETTER(int, next_id);

    bool LoadFromFile();
    bool StoreToFile();

    int PutAvatar(AnimatedAvatar *avatar, bool *ok);

    inline const AnimatedAvatar *FindOrNull(int avatar_id) const;
    inline AnimatedAvatar *FindMutableOrNull(int avatar_id);

    inline size_t GetAllAvatarIdentifiers(std::vector<int> *ids) const;

    int NextId() { return next_id_ += 10; }

    static const char kName[];

    DISALLOW_IMPLICIT_CONSTRUCTORS(AnimatedAvatarStorage);
private:
    std::string dir_ = "assets";
    std::string grid_pic_name_;
    const int start_id_;
    int next_id_;
    std::map<int, AnimatedAvatar *> avatars_;
}; // class AnimatedAvatarStorage

inline const AnimatedAvatar *
AnimatedAvatarStorage::FindOrNull(int avatar_id) const {
    auto iter = avatars_.find(avatar_id);
    return iter == avatars_.end() ? nullptr : iter->second;
}

inline AnimatedAvatar *AnimatedAvatarStorage::FindMutableOrNull(int avatar_id) {
    auto iter = avatars_.find(avatar_id);
    return iter == avatars_.end() ? nullptr : iter->second;
}

inline size_t
AnimatedAvatarStorage::GetAllAvatarIdentifiers(std::vector<int> *ids) const {
    ids->clear();
    for (const auto &pair : avatars_) {
        ids->push_back(pair.first);
    }
    return ids->size();
}

} // namespace utaha

#endif // UTAHA_ANIMATED_AVATAR_STORAGE_H_
