#include "animated-avatar-storage.h"
#include "animated-avatar.h"
#include "glog/logging.h"

namespace utaha {

const char AnimatedAvatarStorage::kName[] = "avatars";

AnimatedAvatarStorage::~AnimatedAvatarStorage() {
    for (const auto &pair : avatars_) {
        delete pair.second;
    }
}

bool AnimatedAvatarStorage::LoadFromFile() {
    std::string metadata_file(dir_);
    metadata_file.append("/").append(kName).append(".metadata");
    if (access(metadata_file.c_str(), F_OK) != 0) {
        return true;
    }

    FILE *f = fopen(metadata_file.c_str(), "rb");
    if (!f) {
        PLOG(ERROR) << "Can not open file: " << metadata_file;
        return false;
    }
    char buf[FILENAME_MAX];
    int start_id = 0;
    fscanf(f, "%s\t%d\n", buf, &start_id);
    if (start_id != start_id_) {
        // TODO:
    }
    grid_pic_name_.assign(buf);
    fclose(f); f = nullptr;

    std::string data_file(dir_);
    data_file.append("/").append(kName).append(".data");
    f = fopen(data_file.c_str(), "rb");
    if (!f) {
        PLOG(ERROR) << "Can not open file: " << data_file;
        return false;
    }

    int avatar_id = 0, w = 0, h = 0;
    while (fscanf(f, "%d\t%s\t%d\t%d", &avatar_id, buf, &w, &h) != EOF) {
        auto avatar = new AnimatedAvatar();
        avatar->set_id(avatar_id);
        avatar->set_name(buf);
        avatar->set_w(w);
        avatar->set_h(h);

        int tex_id = 0;
        for (int i = 0; i < MAX_DIR; ++i) {
            for (int j = 0; j < AnimatedAvatar::Animation::kNumFrames; ++j) {
                if (fscanf(f, "\t%d", &tex_id) == EOF) {
                    delete avatar;
                    return false;
                }
                avatar->mutable_animation(static_cast<Direction>(i))
                      ->set_frame(j, tex_id);
            }
        }
        fscanf(f, "\n");
        bool ok = true;
        PutAvatar(avatar, &ok);

        if (avatar_id > next_id_) {
            next_id_ = avatar_id;
        }
    }
    fclose(f); f = nullptr;

    if (next_id_ != 0) {
        NextId();
    }
    return true;
}

bool AnimatedAvatarStorage::StoreToFile() {
    std::string metadata_file(dir_);
    metadata_file.append("/").append(kName).append(".metadata");
    FILE *f = fopen(metadata_file.c_str(), "wb");
    if (!f) {
        PLOG(ERROR) << "Can not open file: " << metadata_file;
        return false;
    }
    fprintf(f, "%s\t%d\n", grid_pic_name_.c_str(), start_id_);
    fclose(f); f = nullptr;

    std::string data_file(dir_);
    data_file.append("/").append(kName).append(".data");
    f = fopen(data_file.c_str(), "wb");
    if (!f) {
        PLOG(ERROR) << "Can not open file: " << data_file;
        return false;
    }
    for (const auto &pair : avatars_) {
        auto avatar = pair.second;
        fprintf(f, "%d\t\%s\t%d\t%d", avatar->id(), avatar->name().empty() ?
                "[unknown]" : avatar->name().c_str(), avatar->w(), avatar->h());
        for (int i = 0; i < MAX_DIR; ++i) {
            fprintf(f, "\t%d\t%d\t%d",
                    avatar->frame(static_cast<Direction>(i), 0),
                    avatar->frame(static_cast<Direction>(i), 1),
                    avatar->frame(static_cast<Direction>(i), 2));
        }
        fprintf(f, "\n");
    }
    fclose(f); f = nullptr;

    return true;
}

int AnimatedAvatarStorage::PutAvatar(AnimatedAvatar *avatar, bool *ok) {
    bool gen_id = false;
    if (avatar->id() == 0) {
        avatar->set_id(next_id_);
        gen_id = true;
    }

    auto iter = avatars_.find(avatar->id());
    if (iter != avatars_.end()) {
        *ok = false;
        delete iter->second;
    } else {
        *ok = true;
    }
    avatars_[avatar->id()] = avatar;
    if (gen_id) {
        NextId();
    }
    return avatar->id();
}



} // namespace utaha
