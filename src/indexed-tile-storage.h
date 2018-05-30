#ifndef UTAHA_INDEXED_TILE_STORAGE_H_
#define UTAHA_INDEXED_TILE_STORAGE_H_

#include "generic-storage.h"
#include "indexed-tile.h"

namespace utaha {

class IndexedTileStorage : public GenericStorage<IndexedTile, 10> {
public:
    IndexedTileStorage(int start_id);
    virtual ~IndexedTileStorage();

    virtual bool LoadFromFile(Original *fs) override;
    virtual bool StoreToFile(Original *fs) const override;
    virtual std::string name() const override;

    DISALLOW_IMPLICIT_CONSTRUCTORS(IndexedTileStorage);
}; // class IndexedTileStorage


} // namespace utaha

#endif // UTAHA_INDEXED_TILE_STORAGE_H_
