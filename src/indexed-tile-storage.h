#ifndef UTAHA_INDEXED_TILE_STORAGE_H_
#define UTAHA_INDEXED_TILE_STORAGE_H_

#include "base.h"
#include <map>
#include <vector>
#include <string>

namespace utaha {

class IndexedTile;
class Original;

class IndexedTileStorage {
public:
    IndexedTileStorage(int start_id)
        : start_id_(start_id)
        , next_id_(start_id) {}
    ~IndexedTileStorage();

    DEF_VAL_PROP_RW(std::string, dir);
    DEF_VAL_PROP_RW(std::string, grid_pic_name);
    DEF_VAL_GETTER(int, next_id);

    bool LoadFromFile(Original *fs);
    bool StoreToFile(Original *fs);

    int PutTile(IndexedTile *tile, bool *ok);

    inline const IndexedTile *FindOrNull(int tile_id) const;
    inline IndexedTile *FindMutableOrNull(int tile_id);

    inline size_t GetAllTileIdentifiers(std::vector<int> *ids) const;

    int NextId() { return next_id_ += 10; }

    static const char kName[];

    DISALLOW_IMPLICIT_CONSTRUCTORS(IndexedTileStorage);
private:
    std::string dir_;
    std::string grid_pic_name_;
    const int start_id_;
    int next_id_;
    std::map<int, IndexedTile *> tiles_;
}; // class IndexedTileStorage

inline const IndexedTile *IndexedTileStorage::FindOrNull(int tile_id) const {
    auto iter = tiles_.find(tile_id);
    return iter == tiles_.end() ? nullptr : iter->second;
}

inline IndexedTile *IndexedTileStorage::FindMutableOrNull(int tile_id) {
    auto iter = tiles_.find(tile_id);
    return iter == tiles_.end() ? nullptr : iter->second;
}

inline size_t
IndexedTileStorage::GetAllTileIdentifiers(std::vector<int> *ids) const {
    ids->clear();
    for (const auto &pair : tiles_) {
        ids->push_back(pair.first);
    }
    return ids->size();
}

} // namespace utaha

#endif // UTAHA_INDEXED_TILE_STORAGE_H_
