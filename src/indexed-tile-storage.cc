#include "indexed-tile-storage.h"
#include "indexed-tile.h"
#include "glog/logging.h"
#include <stdio.h>

namespace utaha {

const char IndexedTileStorage::kName[] = "tiles";

IndexedTileStorage::~IndexedTileStorage() {
    for (const auto &pair : tiles_) {
        delete pair.second;
    }
}

/**
 * ${name}.metadata
 * [grid-pic-name]  [start-id]
 *
 * ${name}.data
 * [id] [name] [tex-id] [passable]
 */
bool IndexedTileStorage::LoadFromFile() {
    std::string metadata_file(dir_);
    metadata_file.append("/").append(kName).append(".metadata");

    FILE *f = fopen(metadata_file.c_str(), "rb");
    if (!f) {
        PLOG(ERROR) << "Ca not open file: " << metadata_file;
        return false;
    }
    char buf[FILENAME_MAX]; int start_id;
    fscanf(f, "%s\t%d\n", buf, &start_id);
    grid_pic_name_.assign(buf);
    if (start_id != start_id_) {
        // TODO:
    }
    fclose(f); f = nullptr;

    std::string data_file(dir_);
    data_file.append("/").append(kName).append(".data");

    f = fopen(data_file.c_str(), "rb");
    if (!f) {
        PLOG(ERROR) << "Can not open file: " << data_file;
        return false;
    }
    int tile_id, tex_id;
    uint32_t passable;
    while (fscanf(f, "%d\t%s\t%d\t%u\n", &tile_id, buf, &tex_id, &passable) != EOF) {
        auto tile = new IndexedTile();
        tile->set_id(tile_id);
        tile->set_name(buf);
        tile->set_tex_id(tex_id);
        tile->set_passable(passable);
        bool ok = true;
        PutTile(tile, &ok);
        if (!ok) {
            delete tile;
        }
    }
    fclose(f); f = nullptr;
    return true;
}

bool IndexedTileStorage::StoreToFile() {
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
    for (const auto &pair : tiles_) {
        fprintf(f, "%d\t%s\t%d\t%u\n", pair.second->id(),
                pair.second->name().empty() ? "[unknown]"
                : pair.second->name().c_str(),
                pair.second->tex_id(), pair.second->passable());
    }
    fclose(f); f = nullptr;

    return true;
}

int IndexedTileStorage::PutTile(IndexedTile *tile, bool *ok) {
    bool gen_id = false;
    if (tile->id() == 0) {
        tile->set_id(next_id_);
        gen_id = true;
    }

    auto iter = tiles_.find(tile->id());
    if (iter != tiles_.end()) {
        *ok = false;
        return iter->first;
    }

    tiles_[tile->id()] = tile;
    if (gen_id) {
        NextId();
    }
    *ok = true;
    return tile->id();
}

} // namespace utaha
