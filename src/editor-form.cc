#include "ui-form.h"
#include "ui-flat-status-bar.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-menu.h"
#include "ui-flat-input-box.h"
#include "ui-flat-check-box.h"
#include "ui-pic-grid-selector.h"
#include "ui-layout.h"
#include "ui-style-collection.h"
#include "ui-component-factory.h"
#include "ui-component-builder.h"
#include "raw-pic-collection.h"
#include "grid-pic-storage.h"
#include "indexed-tile-storage.h"
#include "indexed-tile.h"
#include "lua-utils.h"
#include "script-executor.h"
#include "glog/logging.h"

namespace utaha {

class RawPicController;
class TileController;

class EditorForm : public UIForm {
public:
    enum Mode {
        EDIT_TILE,
        EDIT_SPIRIT,
        EDIT_MAP,
    };

    EditorForm();
    virtual ~EditorForm();

    virtual int OnCommand(UIComponent *sender, int cmd_id, void *param,
                          bool *is_break) override;
    virtual int OnEditChanged(UIComponent *sender, bool *is_break) override;

    virtual int OnInit() override;
    virtual void OnEvent(SDL_Event *e, bool *is_break) override;
    virtual void OnAfterRender() override;
    virtual void OnQuit() override;

    DISALLOW_IMPLICIT_CONSTRUCTORS(EditorForm);
private:
    UIStyleCollection *styles_ = nullptr;
    UIComponentFactory *factory_ = nullptr;
    RawPicCollection *raw_pics_ = nullptr;
    SDL_Texture *texture_ = nullptr;

    UILayout *right_layout_ = nullptr;
    UILayout *tile_layout_ = nullptr;

    Mode mode_ = EDIT_TILE;
    RawPicController *raw_pic_ctrl_ = nullptr;
    TileController   *tile_ctrl_ = nullptr;

    IndexedTileStorage *tiles_ = nullptr;
    GridPicStorage *tiles_tex_ = nullptr;
}; // class EditorForm

#define DEFINE_CMD_ID(M) \
    M(ID_FILE_TILE,         100) \
    M(ID_FILE_SPIRIT,       200) \
    M(ID_FILE_MAP,          300) \
    M(ID_FILE_SAVE_ALL,     400) \
    M(ID_TILE_NEW,          110) \
    M(ID_TILE_COMMIT,       120) \
    M(ID_TILE_NEXT,         130) \
    M(ID_TILE_PREV,         140)

struct EditorFormR {
    enum ID: int {
        DEFINE_CMD_ID(LUA_DEF_CONST_ENUM)
    };
};

////////////////////////////////////////////////////////////////////////////////
// class RawPicController
////////////////////////////////////////////////////////////////////////////////

class RawPicController {
public:
    RawPicController() = default;
    ~RawPicController() = default;

    DEF_PTR_PROP_RW_NOTNULL2(UIPicGridSelector, selector);
    DEF_PTR_PROP_RW_NOTNULL2(RawPicCollection, raw_pics);
    DEF_VAL_PROP_RMW(std::vector<std::string>, raw_files);

    bool NextFile() {
        current_raw_file_ = (current_raw_file_ + 1) % raw_files_.size();
        return SetCurrentPic(raw_files_[current_raw_file_]);
    }

    bool PrevFile() {
        if (current_raw_file_ == 0) {
            current_raw_file_ = raw_files_.size() - 1;
        } else {
            --current_raw_file_;
        }
        return SetCurrentPic(raw_files_[current_raw_file_]);
    }

    std::string CurrentFile() { return raw_files_[current_raw_file_]; }

    bool Reset() {
        current_raw_file_ = 0;
        return SetCurrentPic(raw_files_[current_raw_file_]);
    }

    DISALLOW_IMPLICIT_CONSTRUCTORS(RawPicController);
private:
    bool SetCurrentPic(const std::string &file) {
        SDL_Surface *surface = raw_pics_->FindPicOrNull(file.c_str());
        if (!surface) {
            return false;
        }
        selector_->SetPic(surface, false);
        selector_->mutable_rect()->w = surface->w;
        selector_->mutable_rect()->h = surface->h;
        return true;
    }

    UIPicGridSelector *selector_;
    std::vector<std::string> raw_files_;
    size_t current_raw_file_ = 0;
    RawPicCollection *raw_pics_ = nullptr;
}; // class RawPicController

////////////////////////////////////////////////////////////////////////////////
// class TileController
////////////////////////////////////////////////////////////////////////////////
class TileController {
public:
    TileController() = default;
    ~TileController() = default;

    DEF_PTR_PROP_RW_NOTNULL2(UIFlatInputBox, tile_id_ib);
    DEF_PTR_PROP_RW_NOTNULL2(UIFlatInputBox, clipping_ib);
    DEF_PTR_PROP_RW_NOTNULL2(IndexedTileStorage, tiles);
    DEF_PTR_PROP_RW_NOTNULL2(GridPicStorage, tiles_tex);

    void AddPassMaskCb(UIFlatCheckBox *cb) {
        pass_mask_cbs_.push_back(DCHECK_NOTNULL(cb));
    }

    void NewTile() {
        if (tile_) {
            delete tile_; tile_ = nullptr;
        } else {
            tile_ = new IndexedTile();
        }

        char buf[128];
        snprintf(buf, arraysize(buf), "%d", tiles_->next_id());
        tile_id_ib_->set_text(buf);
        tile_id_ib_->changed();
    }

    void CommitTile(const std::string &file_name, UIPicGridSelector *selector) {
        if (!tile_) {
            return;
        }
        if (!selector->is_selected()) {
            return;
        }

        int clipping = atoi(clipping_ib_->text().c_str());
        int tex_id = 0;
        bool ok = true;
        if (!tiles_tex_->FindOrNullGrid(file_name,
                                        selector->selected_index(), &tex_id)) {
            tex_id = tiles_tex_->PutGrid(file_name,
                                         selector->selected_index(),
                                         selector->CutSelectedSurface(clipping),
                                         &ok);
        }

        uint32_t passable = 0;
        if (pass_mask_cbs_[0]->checked()) {
            passable |= IndexedTile::kWalkPass;
        }
        if (pass_mask_cbs_[1]->checked()) {
            passable |= IndexedTile::kFlyPass;
        }
        if (pass_mask_cbs_[2]->checked()) {
            passable |= IndexedTile::kBulletPass;
        }
        if (pass_mask_cbs_[3]->checked()) {
            passable |= IndexedTile::kMagicPass;
        }
        tile_->set_passable(passable);
        tile_->set_id(tiles_->next_id());
        tile_->set_tex_id(tex_id);

        tiles_->PutTile(tile_, &ok);
        if (ok) {
            tiles_->NextId();
        }
        tile_ = nullptr;
    }

    DISALLOW_IMPLICIT_CONSTRUCTORS(TileController);
private:
    UIFlatInputBox *tile_id_ib_ = nullptr;
    UIFlatInputBox *clipping_ib_ = nullptr;
    IndexedTile *tile_ = nullptr;
    IndexedTileStorage *tiles_ = nullptr;
    GridPicStorage *tiles_tex_ = nullptr;
    std::vector<UIFlatCheckBox *> pass_mask_cbs_;
};


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

static const LuaConstantId editor_form_id[] = {
    DEFINE_CMD_ID(LUA_DEF_CONST_ID)
};

UIForm *CreateEditorForm() {
    return new EditorForm();
}

EditorForm::EditorForm() {
}

/*virtual*/ EditorForm::~EditorForm() {
}

/*virtual*/ int EditorForm::OnCommand(UIComponent *sender, int cmd_id,
                                      void *param, bool *is_break) {
    switch (cmd_id) {
        case EditorFormR::ID_FILE_TILE:
            mode_ = EDIT_TILE;
            break;

        case EditorFormR::ID_FILE_SPIRIT:
            mode_ = EDIT_SPIRIT;
            break;

        case EditorFormR::ID_FILE_MAP:
            mode_ = EDIT_MAP;
            break;

        case EditorFormR::ID_FILE_SAVE_ALL:
            if (!tiles_tex_->StoreToFile()) {
                LOG(ERROR) << "Store tiles texture fail!";
            }
            if (!tiles_->StoreToFile()) {
                LOG(ERROR) << "Store tiles fail!";
            }
            break;

        case EditorFormR::ID_TILE_NEW:
            tile_ctrl_->NewTile();
            break;

        case EditorFormR::ID_TILE_COMMIT:
            if (!raw_pic_ctrl_->selector()->is_selected()) {
                LOG(ERROR) << "Raw pic grid has not selected!";
                break;
            }
            tile_ctrl_->CommitTile(raw_pic_ctrl_->CurrentFile(),  raw_pic_ctrl_->selector());
            tile_ctrl_->NewTile();
            break;

        default:
            break;
    }
    return 0;
}

/*virtual*/ int EditorForm::OnEditChanged(UIComponent *sender, bool *is_break) {
    if (sender->name().compare("grid-size-w-input") == 0) {
        auto edit = down_cast<UIFlatInputBox>(sender);
        int grid_w = atoi(edit->text().c_str());
        if (grid_w > 0) {
            raw_pic_ctrl_->selector()->set_grid_size_w(grid_w);
            raw_pic_ctrl_->selector()->unselected();
        }
    }
    if (sender->name().compare("grid-size-h-input") == 0) {
        auto edit = down_cast<UIFlatInputBox>(sender);
        int grid_h = atoi(edit->text().c_str());
        if (grid_h > 0) {
            raw_pic_ctrl_->selector()->set_grid_size_h(grid_h);
            raw_pic_ctrl_->selector()->unselected();
        }
    }
    return 0;
}

/*virtual*/ int EditorForm::OnInit() {
    ScriptScope scripts(SCRIPTS.ptr());

    styles_ = new UIStyleCollection();
    std::string err;
    styles_->LoadFromFile("res/styles.lua", &err);
    if (!err.empty()) {
        LOG(ERROR) << "Can not load styles file. " << err;
        return -1;
    }
    factory_ = CreateUIComponentStyleFactory(styles_);
    if (!factory_) {
        LOG(ERROR) << "Can not create ui-component-factory. ";
        return -1;
    }

    UIComponentBuilder *builder = new UIComponentBuilder(factory_, this, this);
    const char *e = nullptr;
    auto L = scripts.ExecStandaloneFile([&builder](lua_State *L){
        UIComponentBuilder::BindTo(L);

        luabridge::getGlobalNamespace(L)
        .beginNamespace(utaha::kLuaNamespace)
            .addVariable("uiBuilder", &builder, false)
        .endNamespace();

        LuaUtils::InitConstantId(L, utaha::kLuaNamespace, "R", editor_form_id,
                                 arraysize(editor_form_id));
    }, "res/form-layouts.lua", &e);
    if (e) {
        LOG(ERROR) << "Can not create form layout. " << e;
        return -1;
    }

    luabridge::LuaRef result = luabridge::LuaRef::fromStack(L, -1);
    if (!result.isTable()) {
        LOG(ERROR) << "Incorrect script return result, need table. "
                   << lua_typename(L, result.type());
        return -1;
    }

    auto main_menu = result["mainMenu"].cast<UIFlatMenuGroup *>();
    set_main_menu(DCHECK_NOTNULL(main_menu));
    auto status_bar = result["statusBar"].cast<UIFlatStatusBar *>();
    set_status_bar(DCHECK_NOTNULL(status_bar));
    right_layout_ = result["rightLayout"].cast<UILayout *>();
    tile_layout_ = result["tileLayout"].cast<UILayout *>();

    int w, h;
    SDL_GetWindowSize(window(), &w, &h);
    status_bar->mutable_rect()->w = w;
    status_bar->mutable_rect()->y = h - status_bar->rect().h;

    raw_pics_ = new RawPicCollection();
    size_t num_raw_pics = 0;
    if ((num_raw_pics = raw_pics_->LoadWithBootstrapScript(
        "res/raw-pic-load.lua", &err)) == 0) {
        LOG(ERROR) << "No any raw pictures loaded!. " << err;
        return -1;
    } else {
        char buf[64];
        snprintf(buf, arraysize(buf), "raw: %lu", num_raw_pics);
        status_bar->mutable_grid(1)->set_text(buf);
    }

    {
        tiles_tex_ = new GridPicStorage();
        tiles_tex_->set_dir("assets");
        tiles_tex_->set_name("tiles-tex");
        tiles_tex_->set_grid_w(48);
        tiles_tex_->set_grid_h(48);
        // TODO: tiles_tex_->LoadFromFile();
    }

    {
        tiles_ = new IndexedTileStorage(1000);
        tiles_->set_dir("assets");
        tiles_->set_grid_pic_name(tiles_tex_->name());
        // TODO: tiles_->LoadFromFile();
    }

    {
        raw_pic_ctrl_ = new RawPicController();
        raw_pics_->GetAllFileNames(raw_pic_ctrl_->mutable_raw_files());
        raw_pic_ctrl_->set_raw_pics(raw_pics_);

        UIPicGridSelector *selector =
            down_cast<UIPicGridSelector>(
                right_layout_->FindComponentOrNull("grid-selector"));
        raw_pic_ctrl_->set_selector(selector);
        raw_pic_ctrl_->Reset();
        status_bar->mutable_grid(2)->set_text(raw_pic_ctrl_->CurrentFile());
    }

    {
        tile_ctrl_ = new TileController();
        auto tile_id_ib = down_cast<UIFlatInputBox>(
            tile_layout_->FindComponentOrNull("tile-id"));
        tile_ctrl_->set_tile_id_ib(tile_id_ib);
        auto clipping_ib = down_cast<UIFlatInputBox>(
            tile_layout_->FindComponentOrNull("tile-clipping"));
        tile_ctrl_->set_clipping_ib(clipping_ib);

        auto cb = down_cast<UIFlatCheckBox>(
            tile_layout_->FindComponentOrNull("walk-pass"));
        tile_ctrl_->AddPassMaskCb(cb);
        cb = down_cast<UIFlatCheckBox>(
            tile_layout_->FindComponentOrNull("fly-pass"));
        tile_ctrl_->AddPassMaskCb(cb);
        cb = down_cast<UIFlatCheckBox>(
            tile_layout_->FindComponentOrNull("bullet-pass"));
        tile_ctrl_->AddPassMaskCb(cb);
        cb = down_cast<UIFlatCheckBox>(
            tile_layout_->FindComponentOrNull("magic-pass"));
        tile_ctrl_->AddPassMaskCb(cb);

        tile_ctrl_->set_tiles(tiles_);
        tile_ctrl_->set_tiles_tex(tiles_tex_);
    }

    UIForm::main_menu()->UpdateRect();
    UIForm::status_bar()->UpdateRect();
    right_layout_->UpdateRect();
    tile_layout_->UpdateRect();
    return UIForm::OnInit();
}

/*virtual*/ void EditorForm::OnEvent(SDL_Event *e, bool *is_break) {
    UIForm::OnEvent(e, is_break);
    if (*is_break) {
        return;
    }

    switch (e->type) {
        case SDL_KEYDOWN:
            break;

        case SDL_KEYUP:
            if (raw_pic_ctrl_->selector()->is_focused()) {
                if (e->key.keysym.sym == SDLK_UP) {
                    raw_pic_ctrl_->PrevFile();
                    status_bar()->mutable_grid(2)
                        ->set_text(raw_pic_ctrl_->CurrentFile());
                    right_layout_->UpdateRect();
                }
                if (e->key.keysym.sym == SDLK_DOWN) {
                    raw_pic_ctrl_->NextFile();
                    status_bar()->mutable_grid(2)
                        ->set_text(raw_pic_ctrl_->CurrentFile());
                    right_layout_->UpdateRect();
                }
            }
            break;

        default:
            break;
    }

    switch (mode_) {
        case EDIT_TILE:
            right_layout_->OnEvent(e, is_break);
            tile_layout_->OnEvent(e, is_break);
            break;

        case EDIT_SPIRIT:
            right_layout_->OnEvent(e, is_break);
            break;

        case EDIT_MAP:
            break;

        default:
            break;
    }
}

/*virtual*/ void EditorForm::OnQuit() {
    delete raw_pic_ctrl_; raw_pic_ctrl_ = nullptr;
    delete tile_ctrl_; tile_ctrl_ = nullptr;
    delete right_layout_; right_layout_ = nullptr;
    delete factory_; factory_ = nullptr;
    delete styles_; styles_ = nullptr;
    delete raw_pics_; raw_pics_ = nullptr;
    delete tiles_; tiles_ = nullptr;
    delete tiles_tex_; tiles_tex_ = nullptr;
}

/*virtual*/ void EditorForm::OnAfterRender() {
    switch (mode_) {
        case EDIT_TILE:
            right_layout_->OnRender(renderer());
            tile_layout_->OnRender(renderer());
            break;

        case EDIT_SPIRIT:
            right_layout_->OnRender(renderer());
            break;

        case EDIT_MAP:
            break;

        default:
            break;
    }


    UIForm::OnAfterRender();
}

} // namespace utaha
