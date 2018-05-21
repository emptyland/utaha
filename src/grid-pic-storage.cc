#include "grid-pic-storage.h"
#include "glog/logging.h"
#include SDL_IMAGE_H
#include <stdio.h>

namespace utaha {

GridPicStorage::GridPicStorage() {
}

GridPicStorage::~GridPicStorage() {
    for (auto surface : grid_pics_) {
        SDL_FreeSurface(surface);
    }
}

/**
 * # ${name}.metadata
 * # grid-w grid-h
 * [grid-w] [grid-h]
 *
 * # ${name}.index
 * [original name] [original index] [index]
 */
bool GridPicStorage::LoadFromFile() {
    std::string meta_file(dir_);
    meta_file.append("/").append(name_).append(".metadata");

    FILE *f = fopen(meta_file.c_str(), "rb");
    if (!f) {
        PLOG(ERROR) << "Can not open file: " << meta_file;
        return false;
    }
    fscanf(f, "%d\t%d\n", &grid_w_, &grid_h_);
    if (ferror(f)) {
        PLOG(ERROR) << "Can not read file: " << meta_file;
        fclose(f);
        return false;
    }
    fclose(f); f = nullptr;

    std::string idx_file(dir_);
    idx_file.append("/").append(name_).append(".index");
    f = fopen(idx_file.c_str(), "rb");
    if (!f) {
        PLOG(ERROR) << "Can not open file: " << idx_file;
        return false;
    }
    char original_file[FILENAME_MAX];
    int original_index = 0, index = 0;
    while (fscanf(f, "%s\t%d\t%d\n", original_file, &original_index,
                  &index) != EOF) {
        char key[FILENAME_MAX];
        snprintf(key, arraysize(key), "%s/%d", original_file, original_index);
        original_[key] = index;
    }
    fclose(f); f = nullptr;

    std::string pic_file(dir_);
    pic_file.append("/").append(name_).append(".png");
    SDL_Surface *whole = IMG_Load(pic_file.c_str());
    if (!whole) {
        LOG(ERROR) << "Can not load img: " << pic_file << " " << SDL_GetError();
        return false;
    }
    if (whole->w % grid_w_) {
        LOG(ERROR) << "Incorrect grid-w or picture w " << grid_w_ << " vs. "
        << whole->w;
        SDL_FreeSurface(whole);
        return false;
    }
    if (whole->h % grid_h_) {
        LOG(ERROR) << "Incorrect grid-h or picture h " << grid_h_ << " vs. "
        << whole->h;
        SDL_FreeSurface(whole);
        return false;
    }

    for (int y = 0; y < whole->h / grid_h_; y++) {
        for (int x = 0; x < whole->w / grid_w_; x++) {
            SDL_Surface *grid = SDL_CreateRGBSurface(0, grid_w_, grid_h_, 32,
                                                     COLOR_R_MASK, COLOR_G_MASK,
                                                     COLOR_B_MASK, COLOR_A_MASK);
            if (!grid) {
                LOG(ERROR) << "Can not create surface." << SDL_GetError();
                SDL_FreeSurface(whole);
                return false;
            }

            SDL_Rect src = { x * grid_w_, y * grid_h_, grid_w_, grid_h_, };
            SDL_UpperBlit(whole, &src, grid, nullptr);
            grid_pics_.push_back(grid);
        }
    }
    SDL_FreeSurface(whole);
    return true;
}

/**
 * ${dir}/${name}.metadata
 * ${dir}/${name}.index
 * ${dir}/${name}.png
 */
bool GridPicStorage::StoreToFile() {
    if (original_.empty()) {
        return true; // TODO:
    }

    std::string meta_file(dir_);
    meta_file.append("/").append(name_).append(".metadata");

    FILE *f = fopen(meta_file.c_str(), "wb");
    if (!f) {
        PLOG(ERROR) << "Can not open file: " << meta_file;
        return false;
    }
    fprintf(f, "%d\t%d\n", grid_w_, grid_h_);
    fclose(f); f = nullptr;

    std::string idx_file(dir_);
    idx_file.append("/").append(name_).append(".index");
    f = fopen(idx_file.c_str(), "wb");
    if (!f) {
        PLOG(ERROR) << "Can not open file: " << meta_file;
        return false;
    }
    for (const auto &pair : original_) {
        auto p = pair.first.rfind("/");
        auto file = pair.first.substr(0, p);
        auto index = pair.first.substr(p + 1);
        fprintf(f, "%s\t%s\t%d\n", file.c_str(), index.c_str(), pair.second);
    }
    fclose(f); f = nullptr;


    int grid_h = grid_pics_.empty() ? 0 : pitch_;
    int grid_v = (static_cast<int>(grid_pics_.size()) + pitch_ - 1) / pitch_;
    int w = grid_h * grid_w_;
    int h = grid_v * grid_h_;
    SDL_Surface *whole = SDL_CreateRGBSurface(0, w, h, 32, COLOR_R_MASK,
                                              COLOR_G_MASK, COLOR_B_MASK,
                                              COLOR_A_MASK);
    if (!whole) {
        LOG(ERROR) << "Can not create surface." << SDL_GetError();
        return false;
    }
    size_t i = 0;
    for (int y = 0; y < grid_v; ++y) {
        for (int x = 0; x < grid_h; ++x) {
            if (i >= grid_pics_.size()) {
                break;
            }
            SDL_Rect dst = {x * grid_w_, y * grid_h_, grid_w_, grid_h_};
            SDL_UpperBlit(grid_pics_[i++], nullptr, whole, &dst);
        }
    }
    SDL_FreeSurface(whole);

    std::string pic_file(dir_);
    pic_file.append("/").append(name_).append(".png");
    if (IMG_SavePNG(whole, pic_file.c_str()) < 0) {
        LOG(ERROR) << "Can not save png file." << SDL_GetError();
        return false;
    }
    return true;
}

int GridPicStorage::PutGrid(const std::string &original_file,
                            int original_idx, SDL_Surface *grid, bool *ok) {
    char key[FILENAME_MAX];
    snprintf(key, arraysize(key), "%s/%d", original_file.c_str(), original_idx);

    auto iter = original_.find(key);
    if (iter != original_.end()) {
        *ok = false;
        SDL_FreeSurface(grid_pics_[iter->second]);
        grid_pics_[iter->second] = grid;
        return iter->second;
    }

    auto index = static_cast<int>(grid_pics_.size());
    grid_pics_.push_back(grid);
    original_[key] = index;
    *ok = true;
    return index;
}

SDL_Surface *GridPicStorage::FindOrNullGrid(const std::string &original_file,
                                            int original_idx, int *index) const {
    char key[FILENAME_MAX];
    snprintf(key, arraysize(key), "%s/%d", original_file.c_str(), original_idx);

    auto iter = original_.find(key);
    if (iter == original_.end()) {
        return nullptr;
    }
    if (index) {
        *index = iter->second;
    }
    return grid_pics_[iter->second];
}

int GridPicStorage::AddGrid(SDL_Surface *grid, bool *ok) {
    return PutGrid("[unknown]", rand() % 10000, grid, ok);
}

} // namespace utaha
