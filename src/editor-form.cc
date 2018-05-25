#include "ui-form.h"
#include "ui-flat-status-bar.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-menu.h"
#include "ui-flat-input-box.h"
#include "ui-flat-check-box.h"
#include "ui-pic-grid-selector.h"
#include "ui-flat-pic-view.h"
#include "ui-layout.h"
#include "ui-style-collection.h"
#include "ui-animated-avatar-view.h"
#include "ui-component-factory.h"
#include "ui-component-builder.h"
#include "universal-profile.h"
#include "animated-avatar-storage.h"
#include "animated-avatar.h"
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
class SpiritController;

class EditorForm : public UIForm {
public:
    enum Mode {
        EDIT_TILE,
        EDIT_SPIRIT,
        EDIT_MAP,
    };

    explicit EditorForm(const UniversalProfile *profile);
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
    const UniversalProfile *profile_;
    UIStyleCollection *styles_ = nullptr;
    UIComponentFactory *factory_ = nullptr;
    RawPicCollection *raw_pics_ = nullptr;
    SDL_Texture *texture_ = nullptr;

    UILayout *right_layout_ = nullptr;
    UILayout *tile_layout_ = nullptr;
    UILayout *spirit_layout_ = nullptr;

    Mode mode_ = EDIT_TILE;
    RawPicController *raw_pic_ctrl_ = nullptr;
    TileController   *tile_ctrl_ = nullptr;
    SpiritController *spirit_ctrl_ = nullptr;

    IndexedTileStorage *tiles_ = nullptr;
    GridPicStorage *tiles_tex_ = nullptr;

    AnimatedAvatarStorage *spirits_ = nullptr;
    GridPicStorage *spirits_tex_ = nullptr;
}; // class EditorForm

#define DEFINE_CMD_ID(M) \
    M(ID_FILE_TILE,          100) \
    M(ID_FILE_SPIRIT,        200) \
    M(ID_FILE_MAP,           300) \
    M(ID_FILE_SAVE_ALL,      400) \
    M(ID_TILE_NEW,           110) \
    M(ID_TILE_COMMIT,        120) \
    M(ID_TILE_NEXT,          130) \
    M(ID_TILE_PREV,          140) \
    M(ID_SELECTOR_UNSELECT,  150) \
    M(ID_SELECTOR_SELECTED,  160) \
    M(ID_SPIRIT_ADD_REGULAR, 210) \
    M(ID_SPIRIT_NEXT,        220) \
    M(ID_SPIRIT_PREV,        230)

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
    DEF_PTR_PROP_RW_NOTNULL2(UIFlatPicView, picked_grid);
    DEF_PTR_PROP_RW_NOTNULL2(IndexedTileStorage, tiles);
    DEF_PTR_PROP_RW_NOTNULL2(GridPicStorage, tiles_tex);

    void AddPassMaskCb(UIFlatCheckBox *cb) {
        pass_mask_cbs_.push_back(DCHECK_NOTNULL(cb));
    }

    const IndexedTile *current_tile() const { return tile_; }

    size_t UpdateTileIds() { return tiles_->GetAllTileIdentifiers(&tile_ids_); }

    void Reset() {
        auto n_tiles = UpdateTileIds();
        if (n_tiles == 0) {
            NewTile();
        } else {
            current_tile_p_ = 0;
            SetCurrentTile();
        }
    }

    void NextTile() {
        current_tile_p_ = (current_tile_p_ + 1) % tile_ids_.size();
        SetCurrentTile();
        new_tile_ = false;
    }

    void PrevTile() {
        if (current_tile_p_ == 0) {
            current_tile_p_ = tile_ids_.size() - 1;
        } else {
            --current_tile_p_;
        }
        SetCurrentTile();
        new_tile_ = false;
    }

    void SetCurrentTile() {
        int tile_id = tile_ids_[current_tile_p_];
        auto tile = tiles_->FindMutableOrNull(tile_id);
        if (!tile) {
            return;
        }
        tile->CopyTo(tile_);

        tile_id_ib_->SetInt(tile_->id());

        if (tile_->walk_pass()) {
            pass_mask_cbs_[0]->set_checked(true);
        } else {
            pass_mask_cbs_[0]->set_checked(false);
        }
        if (tile_->fly_pass()) {
            pass_mask_cbs_[1]->set_checked(true);
        } else {
            pass_mask_cbs_[1]->set_checked(false);
        }
        if (tile_->bullet_pass()) {
            pass_mask_cbs_[2]->set_checked(true);
        } else {
            pass_mask_cbs_[2]->set_checked(false);
        }
        if (tile_->magic_pass()) {
            pass_mask_cbs_[3]->set_checked(true);
        } else {
            pass_mask_cbs_[3]->set_checked(false);
        }

        picked_grid_->SetPic(tiles_tex_->FindOrNullGrid(tile_->tex_id()), false);
    }

    void NewTile() {
        new_tile_ = true;
        tile_id_ib_->SetInt(tiles_->next_id());
        picked_grid_->SetPic(nullptr, false);
    }

    void OnSelectRawGrid(UIPicGridSelector *selector) {
        if (selector->is_selected()) {
            int clipping = atoi(clipping_ib_->text().c_str());
            picked_grid_->SetPic(selector->CutSelectedSurface(clipping), true);
        } else {
            picked_grid_->SetPic(nullptr, false);
        }
    }

    void CommitTile(const std::string &file_name, UIPicGridSelector *selector) {
        UpdateTile(file_name, selector);
        bool ok = true;
        tiles_->PutTile(tile_, &ok);
        if (new_tile_) {
            tiles_->NextId();
            tile_ids_.push_back(tile_->id());
            current_tile_p_ = tile_ids_.size() - 1;
        }
        picked_grid_->SetPic(tiles_tex_->FindOrNullGrid(tile_->tex_id()), false);
        tile_ = new IndexedTile();
    }

    void UpdateTile(const std::string &file_name, UIPicGridSelector *selector) {
        if (selector->is_selected()) {
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
            tile_->set_tex_id(tex_id);
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
    }

    DISALLOW_IMPLICIT_CONSTRUCTORS(TileController);
private:
    UIFlatInputBox *tile_id_ib_ = nullptr;
    UIFlatInputBox *clipping_ib_ = nullptr;
    UIFlatPicView *picked_grid_ = nullptr;
    IndexedTile *tile_ = new IndexedTile();
    bool new_tile_ = false;
    IndexedTileStorage *tiles_ = nullptr;
    GridPicStorage *tiles_tex_ = nullptr;
    std::vector<UIFlatCheckBox *> pass_mask_cbs_;
    std::vector<int> tile_ids_;
    size_t current_tile_p_ = 0;
}; // class TileController


////////////////////////////////////////////////////////////////////////////////
// class SpiritController
////////////////////////////////////////////////////////////////////////////////

class SpiritController {
public:
    inline SpiritController() = default;
    ~SpiritController() { delete spirit_; }

    DEF_PTR_PROP_RW_NOTNULL2(UIFlatInputBox, spirit_id_ib);
    DEF_PTR_PROP_RW_NOTNULL2(UIFlatInputBox, spirit_name_ib);
    DEF_PTR_PROP_RW_NOTNULL2(UIFlatInputBox, animated_speed_ib);
    DEF_PTR_PROP_RW_NOTNULL2(UIAnimatedAvatarView, spirit_view);
    DEF_PTR_PROP_RW_NOTNULL2(AnimatedAvatarStorage, spirits);
    DEF_PTR_PROP_RW_NOTNULL2(GridPicStorage, spirits_tex);

    void Reset() {
        auto n_spirits = spirits_->GetAllAvatarIdentifiers(&spirit_ids_);
        if (n_spirits == 0) {
            NewSpirit();
        } else {
            current_spirit_p_ = 0;
            SetCurrentSpirit();
        }
        spirit_view_->set_grids(spirits_tex_->mutable_grid_pics());
    }

    void NextSpirit() {
        current_spirit_p_ = (current_spirit_p_ + 1) % spirit_ids_.size();
        SetCurrentSpirit();
        new_spirit_ = false;
    }

    void PrevSpirit() {
        if (current_spirit_p_ == 0) {
            current_spirit_p_ = spirit_ids_.size() - 1;
        } else {
            --current_spirit_p_;
        }
        SetCurrentSpirit();
        new_spirit_ = false;
    }

    void NewSpirit() {
        new_spirit_ = true;
        spirit_name_ib_->SetText("[unnamed]");
        spirit_id_ib_->SetInt(spirits_->next_id());

        spirit_view_->SetAvatar(nullptr, false);
    }

    void SetCurrentSpirit() {
        int spirit_id = spirit_ids_[current_spirit_p_];
        auto spirit = spirits_->FindMutableOrNull(spirit_id);
        if (!spirit) {
            return;
        }
        spirit->CopyTo(spirit_);

        spirit_id_ib_->SetInt(spirit_->id());
        spirit_name_ib_->SetText(spirit_->name());
        spirit_view_->SetAvatar(spirit_, false);
    }

    void AddRegular(const std::string &file_name, UIPicGridSelector *selector) {
        if (selector->max_h_grids() % 3 != 0) {
            return;
        }
        if (selector->max_v_grids() % 4 != 0) {
            return;
        }
        for (int j = 0; j < selector->max_v_grids() / 4; ++j) {
            for (int i = 0; i < selector->max_h_grids() / 3; ++i) {
                MakeSpirit(file_name, selector, i, j);
            }
        }
    }

    void MakeSpirit(const std::string &file_name, UIPicGridSelector *selector,
                    int block_x, int block_y) {
        spirit_->set_name("[unnamed]");
        spirit_->set_w(spirit_view_->view_w());
        spirit_->set_h(spirit_view_->view_h());
        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 3; ++i) {
                int y = block_y * 4 + j;
                int x = block_x * 3 + i;
                bool ok = true;
                auto tex_id = spirits_tex_->PutGrid(
                    file_name,
                    selector->selected_index(x, y),
                    selector->CutSelectedSurface(x, y, 0),
                    &ok);
                spirit_->set_frame(static_cast<Direction>(j), i, tex_id);
            }
        }
        bool ok = true;
        spirits_->PutAvatar(spirit_, &ok);
        spirit_ids_.push_back(spirit_->id());
        current_spirit_p_ = spirit_ids_.size() - 1;

        spirit_view_->SetAvatar(spirit_, false);
        spirit_name_ib_->SetText(spirit_->name());
        spirit_id_ib_->SetInt(spirit_->id());

        spirit_ = new AnimatedAvatar();
    }

    DISALLOW_IMPLICIT_CONSTRUCTORS(SpiritController);
private:
    UIFlatInputBox *spirit_id_ib_ = nullptr;
    UIFlatInputBox *spirit_name_ib_ = nullptr;
    UIFlatInputBox *animated_speed_ib_ = nullptr;
    UIAnimatedAvatarView *spirit_view_ = nullptr;
    AnimatedAvatarStorage *spirits_ = nullptr;
    GridPicStorage *spirits_tex_ = nullptr;
    AnimatedAvatar *spirit_ = new AnimatedAvatar();
    bool new_spirit_ = false;
    size_t current_spirit_p_ = 0;
    std::vector<int> spirit_ids_;
}; // class SpiritController


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

static const LuaConstantId editor_form_id[] = {
    DEFINE_CMD_ID(LUA_DEF_CONST_ID)
};

UIForm *CreateEditorForm(const UniversalProfile *profile) {
    return new EditorForm(profile);
}

EditorForm::EditorForm(const UniversalProfile *profile)
    : profile_(DCHECK_NOTNULL(profile)) {
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
            if (!spirits_tex_->StoreToFile()) {
                LOG(ERROR) << "Store spirits texture fail!";
            }
            if (!spirits_->StoreToFile()) {
                LOG(ERROR) << "Store spirits fail!";
            }
            break;

        case EditorFormR::ID_TILE_NEXT:
            tile_ctrl_->NextTile();
            break;

        case EditorFormR::ID_TILE_PREV:
            tile_ctrl_->PrevTile();
            break;

        case EditorFormR::ID_TILE_NEW:
            tile_ctrl_->NewTile();
            break;

        case EditorFormR::ID_TILE_COMMIT:
            if (!raw_pic_ctrl_->selector()->is_selected()) {
                LOG(ERROR) << "Raw pic grid has not selected!";
                break;
            }
            tile_ctrl_->CommitTile(raw_pic_ctrl_->CurrentFile(),
                                   raw_pic_ctrl_->selector());
            tile_ctrl_->NewTile();
            break;

        case EditorFormR::ID_SELECTOR_UNSELECT:
            raw_pic_ctrl_->selector()->unselected();
            break;

        case EditorFormR::ID_SELECTOR_SELECTED:
            tile_ctrl_->OnSelectRawGrid(raw_pic_ctrl_->selector());
            break;

        case EditorFormR::ID_SPIRIT_ADD_REGULAR:
            spirit_ctrl_->AddRegular(raw_pic_ctrl_->CurrentFile(),
                                     raw_pic_ctrl_->selector());
            break;

        case EditorFormR::ID_SPIRIT_NEXT:
            spirit_ctrl_->NextSpirit();
            break;

        case EditorFormR::ID_SPIRIT_PREV:
            spirit_ctrl_->PrevSpirit();
            break;

        default:
            break;
    }
    return 0;
}

/*virtual*/ int EditorForm::OnEditChanged(UIComponent *sender, bool *is_break) {
    if (sender->name().compare("grid-size-w-input") == 0) {
        int grid_w = down_cast<UIFlatInputBox>(sender)->GetInt();
        if (grid_w > 0) {
            raw_pic_ctrl_->selector()->set_grid_size_w(grid_w);
            raw_pic_ctrl_->selector()->unselected();
        }
    }
    if (sender->name().compare("grid-size-h-input") == 0) {
        int grid_h = down_cast<UIFlatInputBox>(sender)->GetInt();
        if (grid_h > 0) {
            raw_pic_ctrl_->selector()->set_grid_size_h(grid_h);
            raw_pic_ctrl_->selector()->unselected();
        }
    }
    if (sender->name().compare("animated-speed") == 0) {
        int animated_speed = down_cast<UIFlatInputBox>(sender)->GetInt();
        if (animated_speed > 0) {
            spirit_ctrl_->spirit_view()->set_animated_speed(animated_speed);
        }
    }
    return 0;
}

/*virtual*/ int EditorForm::OnInit() {
    ScriptScope scripts(SCRIPTS.ptr());

    styles_ = new UIStyleCollection();
    std::string err;
    styles_->LoadFromFile(profile_->editor_styles_file().c_str(), &err);
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
    }, profile_->editor_layout_file().c_str(), &e);
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
    spirit_layout_ = result["spiritLayout"].cast<UILayout *>();

    int w, h;
    SDL_GetWindowSize(window(), &w, &h);
    status_bar->mutable_rect()->w = w;
    status_bar->mutable_rect()->y = h - status_bar->rect().h;

    raw_pics_ = new RawPicCollection();
    size_t num_raw_pics = 0;
    if ((num_raw_pics = raw_pics_->LoadWithBootstrapScript(
        profile_->raw_pic_load_file().c_str(), &err)) == 0) {
        LOG(ERROR) << "No any raw pictures loaded!. " << err;
        return -1;
    } else {
        char buf[64];
        snprintf(buf, arraysize(buf), "raw: %lu", num_raw_pics);
        status_bar->mutable_grid(1)->set_text(buf);
    }

    {
        tiles_tex_ = new GridPicStorage();
        tiles_tex_->set_dir(profile_->assets_dir());
        tiles_tex_->set_name("tiles-tex");
        tiles_tex_->set_grid_w(profile_->tile_w());
        tiles_tex_->set_grid_h(profile_->tile_h());
        if (!tiles_tex_->LoadFromFile()) {
            LOG(ERROR) << "Can not laod: " << tiles_tex_->name();
            return -1;
        }
    }

    {
        tiles_ = new IndexedTileStorage(1000);
        tiles_->set_dir(profile_->assets_dir());
        tiles_->set_grid_pic_name(tiles_tex_->name());
        if (!tiles_->LoadFromFile()) {
            LOG(ERROR) << "Can not laod tiles.";
            return -1;
        }
    }

    {
        spirits_tex_ = new GridPicStorage();
        spirits_tex_ = new GridPicStorage();
        spirits_tex_->set_dir(profile_->assets_dir());
        spirits_tex_->set_name("avatars-tex");
        spirits_tex_->set_grid_w(profile_->avatar_w());
        spirits_tex_->set_grid_h(profile_->avatar_h());
        if (!spirits_tex_->LoadFromFile()) {
            LOG(ERROR) << "Can not laod: " << spirits_tex_->name();
            return -1;
        }
    }

    {
        spirits_ = new AnimatedAvatarStorage(2000);
        spirits_->set_dir(profile_->assets_dir());
        spirits_->set_grid_pic_name(spirits_tex_->name());
        if (!spirits_->LoadFromFile()) {
            LOG(ERROR) << "Can not laod avatars.";
            return -1;
        }
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
        auto picked_grid = down_cast<UIFlatPicView>(
            tile_layout_->FindComponentOrNull("picked-grid"));
        tile_ctrl_->set_picked_grid(picked_grid);

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
        tile_ctrl_->Reset();
    }

    {
        spirit_ctrl_ = new SpiritController();
        auto id = down_cast<UIFlatInputBox>(
            spirit_layout_->FindComponentOrNull("spirit-id"));
        spirit_ctrl_->set_spirit_id_ib(id);
        auto name = down_cast<UIFlatInputBox>(
            spirit_layout_->FindComponentOrNull("spirit-name"));
        spirit_ctrl_->set_spirit_name_ib(name);
        auto speed = down_cast<UIFlatInputBox>(
            spirit_layout_->FindComponentOrNull("animated-speed"));
        spirit_ctrl_->set_animated_speed_ib(speed);
        auto view = down_cast<UIAnimatedAvatarView>(
            spirit_layout_->FindComponentOrNull("spirit-view"));
        spirit_ctrl_->set_spirit_view(view);

        spirit_ctrl_->set_spirits(spirits_);
        spirit_ctrl_->set_spirits_tex(spirits_tex_);
        spirit_ctrl_->Reset();
    }

    UIForm::main_menu()->UpdateRect();
    UIForm::status_bar()->UpdateRect();
    right_layout_->UpdateRect();
    tile_layout_->UpdateRect();
    spirit_layout_->UpdateRect();
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
            if (raw_pic_ctrl_->selector()->focused()) {
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
            if (tile_ctrl_->picked_grid()->focused()) {
                if (e->key.keysym.sym == SDLK_UP) {
                    tile_ctrl_->PrevTile();
                }
                if (e->key.keysym.sym == SDLK_DOWN) {
                    tile_ctrl_->NextTile();
                }
            }
            if (spirit_ctrl_->spirit_view()->focused()) {
                if (e->key.keysym.sym == SDLK_UP) {
                    spirit_ctrl_->PrevSpirit();
                }
                if (e->key.keysym.sym == SDLK_DOWN) {
                    spirit_ctrl_->NextSpirit();
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
            spirit_layout_->OnEvent(e, is_break);
            break;

        case EDIT_MAP:
            break;

        default:
            break;
    }
}

/*virtual*/ void EditorForm::OnQuit() {
    delete raw_pic_ctrl_; raw_pic_ctrl_ = nullptr;
    delete spirit_ctrl_; spirit_ctrl_ = nullptr;
    delete tile_ctrl_; tile_ctrl_ = nullptr;
    delete right_layout_; right_layout_ = nullptr;
    delete spirit_layout_; spirit_layout_ = nullptr;
    delete factory_; factory_ = nullptr;
    delete styles_; styles_ = nullptr;
    delete raw_pics_; raw_pics_ = nullptr;
    delete tiles_; tiles_ = nullptr;
    delete tiles_tex_; tiles_tex_ = nullptr;
    delete spirits_; spirits_ = nullptr;
    delete spirits_tex_; spirits_tex_ = nullptr;
}

/*virtual*/ void EditorForm::OnAfterRender() {
    switch (mode_) {
        case EDIT_TILE:
            right_layout_->OnRender(renderer());
            tile_layout_->OnRender(renderer());
            break;

        case EDIT_SPIRIT:
            right_layout_->OnRender(renderer());
            spirit_layout_->OnRender(renderer());
            break;

        case EDIT_MAP:
            break;

        default:
            break;
    }


    UIForm::OnAfterRender();
}

} // namespace utaha
