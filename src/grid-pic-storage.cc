#include "grid-pic-storage.h"
#include "glog/logging.h"
#include "base-io.h"
#include SDL_IMAGE_H
#include <stdio.h>
#include <memory>

namespace utaha {

GridPicStorage::GridPicStorage() {
}

/*virtual*/ GridPicStorage::~GridPicStorage() {
    for (auto surface : grid_pics_) {
        SDL_FreeSurface(surface);
    }
}

/*virtual*/ std::string GridPicStorage::name() const { return name_; }

/**
 * # ${name}.metadata
 * # grid-w grid-h
 * [grid-w] [grid-h]
 *
 * # ${name}.index
 * [original name] [original index] [index]
 */
/*virtual*/ bool GridPicStorage::LoadFromFile(Original *fs) {
    std::string file = Original::sprintf("%s/%s.metadata", dir_.c_str(),
                                         name_.c_str());
    if (fs->FileNotExist(file)) {
        return true;
    }

    std::unique_ptr<FileTextInputStream> f(fs->OpenTextFileRd(file));
    if (!f) {
        return false;
    }
    size_t n_grids = 0;
    f->Scanf("%d\t%d\t%zd\n", &grid_w_, &grid_h_, &n_grids);

    file = Original::sprintf("%s/%s.index", dir_.c_str(), name_.c_str());
    f.reset(fs->OpenTextFileRd(file));
    if (!f) {
        return false;
    }
    char original_file[FILENAME_MAX];
    int original_index = 0, index = 0;
    while (f->Scanf("%s\t%d\t%d\n", original_file, &original_index,
                    &index) != EOF) {
        auto key = Original::sprintf("%s/%d", original_file, original_index);
        original_[key] = index;
    }

    file = Original::sprintf("%s/%s.png", dir_.c_str(), name_.c_str());
    SDL_Surface *whole = IMG_Load(file.c_str());
    if (!whole) {
        LOG(ERROR) << "Can not load img: " << file << " " << SDL_GetError();
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
            if (n_grids == 0) {
                goto final;
            }
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

            --n_grids;
        }
    }
final:
    SDL_FreeSurface(whole);
    return true;
}

/**
 * ${dir}/${name}.metadata
 * ${dir}/${name}.index
 * ${dir}/${name}.png
 */
/*virtual*/ bool GridPicStorage::StoreToFile(Original *fs) const {
    if (original_.empty()) {
        return true; // TODO:
    }

    std::string file = Original::sprintf("%s/%s.metadata", dir_.c_str(),
                                         name_.c_str());
    std::unique_ptr<FileTextOutputStream> f(fs->OpenTextFileWr(file));
    if (!f) {
        return false;
    }
    f->Printf("%d\t%d\t%zd\n", grid_w_, grid_h_, grid_pics_.size());

    file = Original::sprintf("%s/%s.index", dir_.c_str(), name_.c_str());
    f.reset(fs->OpenTextFileWr(file));
    if (!f) {
        return false;
    }
    for (const auto &pair : original_) {
        auto p = pair.first.rfind("/");
        auto file = pair.first.substr(0, p);
        auto index = pair.first.substr(p + 1);
        f->Printf("%s\t%s\t%d\n", file.c_str(), index.c_str(), pair.second);
    }

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

    file = Original::sprintf("%s/%s.png", dir_.c_str(), name_.c_str());
    if (IMG_SavePNG(whole, file.c_str()) < 0) {
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
