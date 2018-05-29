#ifndef UTAHA_GENERIC_STORAGE_H_
#define UTAHA_GENERIC_STORAGE_H_

#include "base.h"
#include <map>
#include <string>
#include <vector>

namespace utaha {

class Original;

class Storage {
public:
    Storage();
    virtual ~Storage();

    virtual bool LoadFromFile(Original *fs) = 0;
    virtual bool StoreToFile(Original *fs) const = 0;

    virtual std::string name() const = 0;

    DISALLOW_IMPLICIT_CONSTRUCTORS(Storage);
}; // class Storage

template <class T, int ID_INC = 10>
class GenericStorage : public Storage {
public:
    GenericStorage(int start_id)
        : start_id_(start_id)
        , next_id_(start_id) {}
    virtual ~GenericStorage();

    DEF_VAL_GETTER(int, start_id);
    DEF_VAL_PROP_RW(int, next_id);
    DEF_VAL_PROP_RW(std::string, dir);
    DEF_VAL_PROP_RW(std::string, tex_name);

    size_t entity_size() const { return entities_.size(); }

    int Put(T *entity, bool *ok);

    inline const T *FindOrNull(int rid) const;
    inline T *FindMutableOrNull(int rid);

    inline size_t GetAllIdentifiers(std::vector<int> *ids) const;

    inline int NextId() { return next_id_ += ID_INC; }

    DISALLOW_IMPLICIT_CONSTRUCTORS(GenericStorage);
protected:
    int next_id_;
    std::map<int, T *> entities_;

private:
    const int start_id_;
    std::string dir_;
    std::string tex_name_;
}; // class GenericStorage

template <class T, int INC>
/*virtual*/ GenericStorage<T, INC>::~GenericStorage() {
    for (const auto &pair : entities_) {
        delete pair.second;
    }
}

template <class T, int INC>
int GenericStorage<T, INC>::Put(T *entity, bool *ok) {
    bool gen_id = false;
    if (entity->id() == 0) {
        entity->set_id(next_id_);
        gen_id = true;
    }

    auto iter = entities_.find(entity->id());
    if (iter != entities_.end()) {
        *ok = false;
        delete iter->second;
    } else {
        *ok = true;
    }
    entities_[entity->id()] = entity;
    if (gen_id) {
        NextId();
    }
    return entity->id();
}

template <class T, int INC>
inline const T *GenericStorage<T, INC>::FindOrNull(int rid) const {
    auto iter = entities_.find(rid);
    return iter == entities_.end() ? nullptr : iter->second;
}

template <class T, int INC>
inline T *GenericStorage<T, INC>::FindMutableOrNull(int rid) {
    auto iter = entities_.find(rid);
    return iter == entities_.end() ? nullptr : iter->second;
}

template <class T, int INC>
inline size_t
GenericStorage<T, INC>::GetAllIdentifiers(std::vector<int> *ids) const {
    ids->clear();
    for (const auto &pair : entities_) {
        ids->push_back(pair.first);
    }
    return ids->size();
}

} // namespace utaha

#endif // UTAHA_GENERIC_STORAGE_H_
