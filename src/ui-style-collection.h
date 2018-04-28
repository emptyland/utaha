#ifndef UTAHA_UI_STYLE_COLLECTION_H_
#define UTAHA_UI_STYLE_COLLECTION_H_

#include "base.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>
#include <tuple>

typedef struct _TTF_Font TTF_Font;
typedef struct lua_State lua_State;

namespace utaha {

union UIStyleValue {
    int       ival;
    float     fval;
    SDL_Color color;
    TTF_Font *font;
    struct {
        int w;
        int h;
    } size;
}; // union UIStyleValue

class UIStyleCollection {
public:
    typedef UIStyleValue Value;

    UIStyleCollection();
    ~UIStyleCollection();

    bool AddFont(const std::string &name, const std::string &ttf_file_path,
                 int size);

    inline bool SetFont(const std::string &name, const std::string &font_name);

    void SetInt(const std::string &name, int val) {
        values_.emplace(name, ForInt(val));
    }

    void SetFloat(const std::string &name, float val) {
        values_.emplace(name, ForFloat(val));
    }

    void SetColor(const std::string &name, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        values_.emplace(name, ForColor(r, g, b, a));
    }

    void SetSize(const std::string &name, int w, int h) {
        values_.emplace(name, ForSize(w, h));
    }

    inline int FindInt(const std::string &name, bool *ok) const;
    inline float FindFloat(const std::string &name, bool *ok) const;
    inline TTF_Font *FindFont(const std::string &name, bool *ok) const;
    inline bool FindColor(const std::string &name, SDL_Color *color) const;
    inline std::tuple<int, int> FindSize(const std::string &name, bool *ok) const;

    static int BindTo(lua_State *L);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIStyleCollection);
private:
    static inline Value ForInt(int val) { return {.ival = val}; }
    static inline Value ForFloat(float val) { return {.fval = val}; }
    static inline Value ForFont(TTF_Font *font) { return {.font = font}; }
    static inline Value ForColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        return {.color = {.r = r, .g = g, .b = b, .a = a}};
    }
    static inline Value ForSize(int w, int h) {
        return {.size = {.w = w, .h = h}};
    }

    int WarpAddFont(lua_State *L);
    int WarpSetFont(lua_State *L);
    int WarpSetColor(lua_State *L);
    UIStyleCollection *WarpSetSize(const char *name, int w, int h);
    UIStyleCollection *WarpSetInt(const char *name, int val);
    UIStyleCollection *WarpSetFloat(const char *name, float val);

    std::unordered_map<std::string, TTF_Font *> fonts_;
    std::unordered_map<std::string, Value> values_;
}; // class UIStyleCollection

inline bool UIStyleCollection::SetFont(const std::string &name,
                                       const std::string &font_name) {
    auto iter = fonts_.find(font_name);
    if (iter == fonts_.end()) {
        return false;
    }
    values_.emplace(name, ForFont(iter->second));
    return true;
}

inline int UIStyleCollection::FindInt(const std::string &name, bool *ok) const {
    auto iter = values_.find(name);
    if (iter == values_.end()) {
        *ok = false;
    } else {
        return iter->second.ival;
    }
    return 0;
}

inline TTF_Font *UIStyleCollection::FindFont(const std::string &name, bool *ok) const {
    auto iter = values_.find(name);
    if (iter == values_.end()) {
        *ok = false;
    } else {
        return iter->second.font;
    }
    return nullptr;
}

inline bool UIStyleCollection::FindColor(const std::string &name,
                                         SDL_Color *color) const {
    auto iter = values_.find(name);
    if (iter == values_.end()) {
        return false;
    } else {
        *color = iter->second.color;
    }
    return true;
}

inline std::tuple<int, int> UIStyleCollection::FindSize(const std::string &name,
                                                        bool *ok) const {
    auto iter = values_.find(name);
    if (iter == values_.end()) {
        *ok = false;
    } else {
        return std::make_tuple(iter->second.size.w, iter->second.size.h);
    }
    return std::make_tuple(0, 0);
}

inline
float UIStyleCollection::FindFloat(const std::string &name, bool *ok) const {
    auto iter = values_.find(name);
    if (iter == values_.end()) {
        *ok = false;
    } else {
        return iter->second.fval;
    }
    return 0;
}

} // namespace utaha

#endif // UTAHA_UI_STYLE_COLLECTION_H_
