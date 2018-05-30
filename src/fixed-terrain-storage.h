#ifndef UTAHA_FIXED_TERRAIN_STORAGE_H_
#define UTAHA_FIXED_TERRAIN_STORAGE_H_

#include "generic-storage.h"
#include "fixed-terrain.h"

namespace utaha {

class FileTextOutputStream;

class FixedTerrainStorage : public GenericStorage<FixedTerrain, 10> {
public:
    FixedTerrainStorage(int start_id);
    virtual ~FixedTerrainStorage();

    virtual bool LoadFromFile(Original *fs) override;
    virtual bool StoreToFile(Original *fs) const override;
    virtual std::string name() const override;

    DISALLOW_IMPLICIT_CONSTRUCTORS(FixedTerrainStorage);
private:
    bool StoreMetadata(const FixedTerrain *terrain, Original *fs) const;
    bool StoreMap(const FixedTerrain *terrain, Original *fs) const;
    bool StoreOthers(const FixedTerrain *terrain, Original *fs) const;
    bool LoadMetadata(FixedTerrain *terrain, Original *fs);
    bool LoadMap(FixedTerrain *terrain, Original *fs);
    bool LoadOthers(FixedTerrain *terrain, Original *fs);
//    bool Store
}; // class FixedTerrainStorage

} // namespace utaha

#endif // UTAHA_FIXED_TERRAIN_STORAGE_H_
