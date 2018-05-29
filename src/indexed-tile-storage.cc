#include "indexed-tile-storage.h"
#include "indexed-tile.h"
#include "glog/logging.h"
#include "base-io.h"
#include <stdio.h>
#include <memory>

namespace utaha {

IndexedTileStorage::IndexedTileStorage(int start_id)
    : GenericStorage(start_id) {
}

/*virtual*/ IndexedTileStorage::~IndexedTileStorage() {
}

/*virtual*/ std::string IndexedTileStorage::name() const { return "tiles"; }

/**
 * ${name}.metadata
 * [grid-pic-name]  [start-id]
 *
 * ${name}.data
 * [id] [name] [tex-id] [passable]
 */
/*virtual*/ bool IndexedTileStorage::LoadFromFile(Original *fs) {
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
    int sid;
    if (f->Scanf("%s\t%d\n", tex_name, &sid) != 2) {
        LOG(ERROR) << "Tile metadata file read fail." << file;
        return false;
    }
    set_tex_name(tex_name);
    if (sid != start_id()) {
        // TODO:
    }

    file = Original::sprintf("%s/%s.data", dir().c_str(), name().c_str());
    f.reset(fs->OpenTextFileRd(file));
    if (!f) {
        return false;
    }
    char name[FILENAME_MAX];
    int tile_id, tex_id;
    uint32_t passable;
    while (f->Scanf("%d\t%s\t%d\t%u\n", &tile_id, name, &tex_id, &passable) != EOF) {
        auto tile = new IndexedTile();
        tile->set_id(tile_id);
        tile->set_name(name);
        tile->set_tex_id(tex_id);
        tile->set_passable(passable);
        bool ok = true;
        Put(tile, &ok);
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

/*virtual*/ bool IndexedTileStorage::StoreToFile(Original *fs) const {
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
        f->Printf("%d\t%s\t%d\t%u\n", pair.second->id(),
                  pair.second->name().empty() ? "[unknown]"
                  : pair.second->name().c_str(),
                  pair.second->tex_id(), pair.second->passable());
    }
    return true;
}

} // namespace utaha
