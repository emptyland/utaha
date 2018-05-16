#include "ui-form.h"
#include "ui-flat-status-bar.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-menu.h"
#include "ui-style-collection.h"
#include "ui-component-factory.h"
#include "ui-component-builder.h"
#include "raw-pic-collection.h"
#include "lua-utils.h"
#include "script-executor.h"
#include "glog/logging.h"

namespace utaha {

class EditorForm : public UIForm {
public:
    EditorForm();
    virtual ~EditorForm();

    virtual int OnCommand(UIComponent *sender, int cmd_id, void *param,
                          bool *is_break) override;

    virtual int OnInit() override;
    virtual void OnAfterRender() override;
//    virtual void OnBeforeRender() override;
    virtual void OnQuit() override;

    DISALLOW_IMPLICIT_CONSTRUCTORS(EditorForm);
private:
    UIStyleCollection *styles_ = nullptr;
    UIComponentFactory *factory_ = nullptr;
    RawPicCollection *raw_pics_ = nullptr;
    SDL_Texture *texture_ = nullptr;
}; // class EditorForm

#define DEFINE_CMD_ID(M) \
    M(ID_EDIT_CONSTRUCTION, 100) \
    M(ID_EDIT_SPIRIT,       200) \
    M(ID_EDIT_MAP,          300) \
    M(ID_FILE_SAVE_ALL,     400)

struct EditorFormR {
    enum ID: int {
        DEFINE_CMD_ID(LUA_DEF_CONST_ENUM)
    };
};

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

/*virtual*/ int EditorForm::OnCommand(UIComponent *sender, int cmd_id, void *param,
                                      bool *is_break) {
    switch (cmd_id) {
        case EditorFormR::ID_EDIT_CONSTRUCTION:
            break;

        case EditorFormR::ID_EDIT_SPIRIT:
            break;

        case EditorFormR::ID_EDIT_MAP: {
            auto grid = static_cast<UIFlatStatusBar *>(status_bar())->mutable_grid(0);
            grid->set_text("Ready");
            grid->set_bg_color({0, 255, 255, 255});
        } break;

        case EditorFormR::ID_FILE_SAVE_ALL: {
            auto grid = static_cast<UIFlatStatusBar *>(status_bar())->mutable_grid(0);
            grid->set_text("Ok");
            grid->set_bg_color({0, 255, 0, 255});
        } break;

        default:
            break;
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

    int w, h;
    SDL_GetWindowSize(window(), &w, &h);
    status_bar->mutable_rect()->w = w;
    status_bar->mutable_rect()->y = h - status_bar->rect().h;

    raw_pics_ = new RawPicCollection();
    size_t num_raw_pics = 0;
    if ((num_raw_pics = raw_pics_->LoadWithBootstrapScript("res/raw-pic-load.lua",
                                                           &err)) == 0) {
        LOG(ERROR) << "No any raw pictures loaded!. " << err;
        return -1;
    } else {
        char buf[64];
        snprintf(buf, arraysize(buf), "raw: %lu", num_raw_pics);
        status_bar->mutable_grid(1)->set_text(buf);
    }
    return UIForm::OnInit();
}

/*virtual*/ void EditorForm::OnQuit() {
    delete factory_; factory_ = nullptr;
    delete styles_; styles_ = nullptr;
    delete raw_pics_; raw_pics_ = nullptr;
}

/*virtual*/ void EditorForm::OnAfterRender() {
    SDL_Surface *surface = raw_pics_->FindPicOrNull("mon3.png");
    if (!texture_) {
        texture_ = SDL_CreateTextureFromSurface(renderer(), surface);
    }

    SDL_Rect dst = {
        0,
        GetRetainTopH(),
        surface->w,
        surface->h,
    };
    SDL_RenderCopy(renderer(), texture_, nullptr, &dst);
    UIForm::OnAfterRender();
}

} // namespace utaha
