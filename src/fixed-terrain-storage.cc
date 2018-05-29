#include "fixed-terrain-storage.h"
#include "base-io.h"
#include <set>

namespace utaha {

FixedTerrainStorage::FixedTerrainStorage(int start_id)
    : GenericStorage(start_id) {
}

/*virtual*/ FixedTerrainStorage::~FixedTerrainStorage() {
}

/*virtual*/ bool FixedTerrainStorage::LoadFromFile(Original *fs) {
    std::string file = Original::sprintf("%s/%s.metadata", dir().c_str(),
                                         name().c_str());
    if (fs->FileNotExist(file)) {
        return true;
    }

    std::unique_ptr<FileTextInputStream> fx(fs->OpenTextFileRd(file));
    if (!fx) {
        return false;
    }
    char tex_name[FILENAME_MAX] = {0};
    int sid = 0, nitems = 0;
    if ((nitems = fx->Scanf("%s\t%d\n", tex_name, &sid)) != 2) {
        LOG(ERROR) << "Terrain metadata read fail. " << file;
        return false;
    }
    set_tex_name(tex_name);
    if (sid != start_id()) {
        // TODO:
    }
    std::set<int> all_map_id;
    int terrain_id = 0;
    while (fx->Scanf("%d\n", &terrain_id) != EOF) {
        all_map_id.insert(terrain_id);
    }

    for (int id : all_map_id) {
        std::unique_ptr<FixedTerrain> terrain(new FixedTerrain());
        terrain->set_id(id);
        if (!LoadMetadata(terrain.get(), fs)) {
            return false;
        }
        if (!LoadMap(terrain.get(), fs)) {
            return false;
        }
        if (!LoadOthers(terrain.get(), fs)) {
            return false;
        }
        if (id > next_id_) {
            next_id_ = id;
        }
        bool ok;
        Put(terrain.release(), &ok);
        DCHECK(ok);
    }
    if (next_id_ > 0) {
        NextId();
    }
    return true;
}

/*virtual*/ bool FixedTerrainStorage::StoreToFile(Original *fs) const {
    std::string file = Original::sprintf("%s/%s.metadata", dir().c_str(),
                                         name().c_str());

    std::unique_ptr<FileTextOutputStream> fx(fs->OpenTextFileWr(file));
    fx->Printf("%s\t%d\n", tex_name().c_str(), start_id());
    for (const auto &pair : entities_) {
        fx->Printf("%d\n", pair.first);
    }
    fx.reset();

    for (const auto &pair : entities_) {
        if (!StoreMetadata(pair.second, fs)) {
            return false;
        }
        if (!StoreMap(pair.second, fs)) {
            return false;
        }
        if (!StoreOthers(pair.second, fs)) {
            return false;
        }
    }
    return true;
}

/*virtual*/ std::string FixedTerrainStorage::name() const {
    return "terrain";
}

bool FixedTerrainStorage::StoreMetadata(const FixedTerrain *terrain,
                                        Original *fs) const {
    std::string file = Original::sprintf("%s/%s-%d.metadata", dir().c_str(),
                                         name().c_str(), terrain->id());
    std::unique_ptr<FileTextOutputStream> fx(fs->OpenTextFileWr(file));
    fx->Printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", terrain->name().c_str(),
               terrain->tile_w(), terrain->tile_h(), terrain->max_h_tiles(),
               terrain->max_v_tiles(), terrain->script_id(),
               terrain->linker_size(), terrain->entity_size(),
               terrain->actor_size());
    return true;
}

bool FixedTerrainStorage::StoreMap(const FixedTerrain *terrain,
                                   Original *fs) const {
    if (!terrain->tiles()) {
        return true;
    }
    std::string file = Original::sprintf("%s/%s-%d.map", dir().c_str(),
                                         name().c_str(), terrain->id());
    std::unique_ptr<FileBinaryOutputStream> fb(fs->OpenBinaryFileWr(file));
    fb->Write(terrain->tiles(), terrain->tile_size() * sizeof(int));
    return true;
}

bool FixedTerrainStorage::StoreOthers(const FixedTerrain *terrain,
                                      Original *fs) const {
    if (terrain->linker_size() > 0) {
        std::string file = Original::sprintf("%s/%s-%d.linker", dir().c_str(),
                                             name().c_str(), terrain->id());
        std::unique_ptr<FileTextOutputStream> fx(fs->OpenTextFileWr(file));
        for (int i = 0; i < terrain->linker_size(); ++i) {
            auto linker = terrain->linker(i);
            fx->Printf("%d\t%d\t%d\t%d\t%d\t%d\n", linker.x, linker.y,
                       linker.params, linker.to_id, linker.to_x,
                       linker.to_y);
        }
    }

    if (terrain->entity_size() > 0) {
        std::string file = Original::sprintf("%s/%s-%d.entity", dir().c_str(),
                                             name().c_str(), terrain->id());
        std::unique_ptr<FileTextOutputStream> fx(fs->OpenTextFileWr(file));
        for (int i = 0; i < terrain->entity_size(); ++i) {
            auto entity = terrain->entity(i);
            fx->Printf("%d\t%d\t%d\n", entity.x, entity.y, entity.spirit_id);
        }
    }

    if (terrain->actor_size() > 0) {
        std::string file = Original::sprintf("%s/%s-%d.actor", dir().c_str(),
                                             name().c_str(), terrain->id());
        std::unique_ptr<FileTextOutputStream> fx(fs->OpenTextFileWr(file));
        for (int i = 0; i < terrain->actor_size(); ++i) {
            auto actor = terrain->actor(i);
            fx->Printf("%d\t%d\t%d\t%d\t%d\n", actor.x, actor.y, actor.param,
                       actor.lv, actor.actor_id);
        }
    }
    return true;
}

bool FixedTerrainStorage::LoadMetadata(FixedTerrain *terrain, Original *fs) {
    std::string file = Original::sprintf("%s/%s-%d.metadata", dir().c_str(),
                                         name().c_str(), terrain->id());
    if (fs->FileNotExist(file)) {
        LOG(ERROR) << "Terrain file: " << file << " not found!";
        return false;
    }

    std::unique_ptr<FileTextInputStream> fx(fs->OpenTextFileRd(file));
    int tile_w, tile_h, max_h_tiles, max_v_tiles, script_id, linker_size,
        entity_size, actor_size;
    char name[FILENAME_MAX] = {0};
    int nitems = fx->Scanf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", name, &tile_w,
                           &tile_h, &max_h_tiles, &max_v_tiles, &script_id,
                           &linker_size, &entity_size, &actor_size);
    if (nitems != 9) {
        LOG(ERROR) << "Can not read terrain metadata file. " << file;
        return false;
    }
    terrain->set_name(name);
    terrain->set_tile_w(tile_w);
    terrain->set_tile_h(tile_h);
    terrain->set_max_h_tiles(max_h_tiles);
    terrain->set_max_v_tiles(max_v_tiles);
    terrain->set_script_id(script_id);
    terrain->set_linker_size(linker_size);
    terrain->set_entity_size(entity_size);
    terrain->set_actor_size(actor_size);
    return true;
}

bool FixedTerrainStorage::LoadMap(FixedTerrain *terrain, Original *fs) {
    std::string file = Original::sprintf("%s/%s-%d.map", dir().c_str(),
                                         name().c_str(), terrain->id());
    if (fs->FileNotExist(file)) {
        LOG(ERROR) << "Terrain file: " << file << " not found!";
        return false;
    }

    size_t bytes = terrain->tile_size() * sizeof(int);
    std::unique_ptr<int[]> tiles(new int[terrain->tile_size()]);
    std::unique_ptr<FileBinaryInputStream> fb(fs->OpenBinaryFileRd(file));
    if (fb->Read(tiles.get(), bytes) != bytes) {
        LOG(ERROR) << "Incorrect map file size, unexpected: " << bytes;
        return false;
    }
    terrain->set_tiles(tiles.release());
    return true;
}

bool FixedTerrainStorage::LoadOthers(FixedTerrain *terrain, Original *fs) {
    if (terrain->linker_size() > 0) {
        std::string file = Original::sprintf("%s/%s-%d.linker", dir().c_str(),
                                             name().c_str(), terrain->id());
        std::unique_ptr<FileTextInputStream> fx(fs->OpenTextFileRd(file));
        if (!fx) {
            LOG(ERROR) << "Can not open terrain's trap file. " << file;
            return false;
        }
        std::unique_ptr<IndexedLinker[]>
            linkers(new IndexedLinker[terrain->linker_size()]);
        for (int i = 0; i < terrain->linker_size(); ++i) {
            int nitems = fx->Scanf("%d\t%d\t%d\t%d\t%d\t%d\n", &linkers[i].x,
                                   &linkers[i].y, &linkers[i].params,
                                   &linkers[i].to_id, &linkers[i].to_x,
                                   &linkers[i].to_y);
            if (nitems != 6) {
                LOG(ERROR) << "Terrain's trap file read fail! line: " << i;
                return false;
            }
        }
        terrain->set_linkers(linkers.release());
    }

    if (terrain->entity_size() > 0) {
        std::string file = Original::sprintf("%s/%s-%d.entity", dir().c_str(),
                                             name().c_str(), terrain->id());
        std::unique_ptr<FileTextInputStream> fx(fs->OpenTextFileRd(file));
        if (!fx) {
            LOG(ERROR) << "Can not open terrain's entity file. " << file;
            return false;
        }
        std::unique_ptr<IndexedEntity[]>
            entities(new IndexedEntity[terrain->entity_size()]);
        for (int i = 0; i < terrain->entity_size(); ++i) {
            int nitems = fx->Scanf("%d\t%d\t%d\n", &entities[i].x,
                                   &entities[i].y, &entities[i].spirit_id);
            if (nitems != 3) {
                LOG(ERROR) << "Terrain's entity file read fail! line: " << i;
                return false;
            }
        }
        terrain->set_entities(entities.release());
    }

    if (terrain->actor_size() > 0) {
        std::string file = Original::sprintf("%s/%s-%d.actor", dir().c_str(),
                                             name().c_str(), terrain->id());
        std::unique_ptr<FileTextInputStream> fx(fs->OpenTextFileRd(file));
        if (!fx) {
            LOG(ERROR) << "Can not open terrain's actor file. " << file;
            return false;
        }
        std::unique_ptr<IndexedActor[]>
            actors(new IndexedActor[terrain->actor_size()]);
        for (int i = 0; i < terrain->actor_size(); ++i) {
            int nitems = fx->Scanf("%d\t%d\t%d\t%d\t%d\n", &actors[i].x,
                                   &actors[i].y, &actors[i].param, &actors[i].lv,
                                   &actors[i].actor_id);
            if (nitems != 5) {
                LOG(ERROR) << "Terrain's actor file read fail! line: " << i;
                return false;
            }
        }
        terrain->set_actors(actors.release());
    }
    return true;
}

} // namespace utaha
