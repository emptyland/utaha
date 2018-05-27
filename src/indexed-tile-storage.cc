#include "indexed-tile-storage.h"
#include "indexed-tile.h"
#include "glog/logging.h"
#include "base-io.h"
#include <stdio.h>
#include <memory>

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
bool IndexedTileStorage::LoadFromFile(Original *fs) {
    std::string metadata_file(dir_);
    metadata_file.append("/").append(kName).append(".metadata");

    if (fs->FileNotExist(metadata_file)) {
        return true;
    }

    std::unique_ptr<FileTextInputStream> f(fs->OpenTextFileRd(metadata_file));
    if (!f) {
        return false;
    }
    char buf[FILENAME_MAX]; int start_id;
    f->Scanf("%s\t%d\n", buf, &start_id);
    grid_pic_name_.assign(buf);
    if (start_id != start_id_) {
        // TODO:
    }

    std::string data_file(dir_);
    data_file.append("/").append(kName).append(".data");

    f.reset(fs->OpenTextFileRd(data_file));
    if (!f) {
        return false;
    }
    int tile_id, tex_id;
    uint32_t passable;
    while (f->Scanf("%d\t%s\t%d\t%u\n", &tile_id, buf, &tex_id, &passable) != EOF) {
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

        if (tile_id > next_id_) {
            next_id_ = tile_id;
        }
    }
    if (next_id_ != 0) {
        NextId();
    }
    return true;
}

bool IndexedTileStorage::StoreToFile(Original *fs) {
    std::string metadata_file(dir_);
    metadata_file.append("/").append(kName).append(".metadata");

    std::unique_ptr<FileTextOutputStream> f(fs->OpenTextFileWr(metadata_file));
    if (!f) {
        return false;
    }
    f->Printf("%s\t%d\n", grid_pic_name_.c_str(), start_id_);

    std::string data_file(dir_);
    data_file.append("/").append(kName).append(".data");
    f.reset(fs->OpenTextFileWr(data_file));
    if (!f) {
        return false;
    }
    for (const auto &pair : tiles_) {
        f->Printf("%d\t%s\t%d\t%u\n", pair.second->id(),
                  pair.second->name().empty() ? "[unknown]"
                  : pair.second->name().c_str(),
                  pair.second->tex_id(), pair.second->passable());
    }
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
        delete iter->second;
    } else {
        *ok = true;
    }
    tiles_[tile->id()] = tile;
    if (gen_id) {
        NextId();
    }
    return tile->id();
}

} // namespace utaha
