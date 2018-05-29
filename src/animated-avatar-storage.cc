#include "animated-avatar-storage.h"
#include "animated-avatar.h"
#include "base-io.h"
#include "glog/logging.h"
#include <memory>

namespace utaha {

AnimatedAvatarStorage::AnimatedAvatarStorage(int start_id)
    : GenericStorage(start_id) {
}

/*virtual*/ AnimatedAvatarStorage::~AnimatedAvatarStorage() {
}

/*virtual*/ std::string AnimatedAvatarStorage::name() const {
    return "avatars";
}

bool AnimatedAvatarStorage::LoadFromFile(Original *fs) {
    std::string file = Original::sprintf("%s/%s.metadata", dir().c_str(),
                                         name().c_str());
    if (fs->FileNotExist(file)) {
        return true;
    }

    std::unique_ptr<FileTextInputStream> f(fs->OpenTextFileRd(file));
    if (!f) {
        return false;
    }
    char tex_name[FILENAME_MAX];
    int sid = 0;
    f->Scanf("%s\t%d\n", tex_name, &sid);
    if (sid != start_id()) {
        // TODO:
    }
    set_tex_name(tex_name);

    file = Original::sprintf("%s/%s.data", dir().c_str(), name().c_str());
    f.reset(fs->OpenTextFileRd(file));
    if (!f) {
        return false;
    }

    char name[FILENAME_MAX];
    int avatar_id = 0, w = 0, h = 0;
    while (f->Scanf("%d\t%s\t%d\t%d", &avatar_id, name, &w, &h) != EOF) {
        auto avatar = new AnimatedAvatar();
        avatar->set_id(avatar_id);
        avatar->set_name(name);
        avatar->set_w(w);
        avatar->set_h(h);

        int tex_id = 0;
        for (int i = 0; i < MAX_DIR; ++i) {
            for (int j = 0;
                j < AnimatedAvatar::Animation::kNumFrames; ++j) {
                if (f->Scanf("\t%d", &tex_id) == EOF) {
                    delete avatar;
                    return false;
                }
                avatar->mutable_animation(static_cast<Direction>(i))
                    ->set_frame(j, tex_id);
            }
        }
        f->Scanf("\n");
        bool ok = true;
        Put(avatar, &ok);
        if (avatar_id > next_id_) {
            next_id_ = avatar_id;
        }
    }
    if (next_id_ != 0) {
        NextId();
    }
    return true;
}

bool AnimatedAvatarStorage::StoreToFile(Original *fs) const {
    std::string file = Original::sprintf("%s/%s.metadata", dir().c_str(),
                                         name().c_str());

    std::unique_ptr<FileTextOutputStream> f(fs->OpenTextFileWr(file));
    if (!f) {
        return false;
    }
    f->Printf("%s\t%d\n", tex_name().c_str(), start_id());

    file = Original::sprintf("%s/%s.data", dir().c_str(), name().c_str());
    f.reset(fs->OpenTextFileWr(file));
    if (!f) {
        return false;
    }
    for (const auto &pair : entities_) {
        auto avatar = pair.second;
        f->Printf("%d\t%s\t%d\t%d", avatar->id(), avatar->name().empty() ?
                  "[unknown]" : avatar->name().c_str(), avatar->w(), avatar->h());
        for (int i = 0; i < MAX_DIR; ++i) {
            f->Printf("\t%d\t%d\t%d",
                      avatar->frame(static_cast<Direction>(i), 0),
                      avatar->frame(static_cast<Direction>(i), 1),
                      avatar->frame(static_cast<Direction>(i), 2));
        }
        f->Printf("\n");
    }
    return true;
}

} // namespace utaha
