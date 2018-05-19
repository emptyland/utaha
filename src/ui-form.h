#ifndef UTAHA_UI_FORM_H_
#define UTAHA_UI_FORM_H_

#include "interactive-listenner.h"
#include "root-render-entity.h"
#include "glog/logging.h"

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef union SDL_Event SDL_Event;

namespace utaha {

class UIComponent;
class UIFlatMenuGroup;
class UIFlatStatusBar;

class UIForm : public InteractiveListenner {
public:
    UIForm();
    virtual ~UIForm();

    virtual int OnClick(UIComponent *sender, bool *is_break) override;
    virtual int OnDoubleClick(UIComponent *sender, bool *is_break) override;
    virtual int OnMouseMove(UIComponent *sender, int x, int y,
                            bool *is_break) override;
    virtual int OnCommand(UIComponent *sender, int cmd_id, void *param,
                          bool *is_break) override;
    virtual int OnEditChanged(UIComponent *sender, bool *is_break) override;

    virtual int  OnInit();
    virtual void OnEvent(SDL_Event *e, bool *is_break);
    virtual void OnAfterRender();
    virtual void OnBeforeRender();
    virtual void OnQuit();

    bool OpenWindow(const char *title, int w, int h);
    int Run();

    int GetRetainTopH() const;
    int GetRetainBottomH() const;

    DEF_PTR_PROP_RW_NOTNULL1(UIFlatMenuGroup, main_menu);
    DEF_PTR_PROP_RW_NOTNULL1(UIFlatStatusBar, status_bar);
    DEF_PTR_PROP_RW_NOTNULL1(SDL_Renderer, renderer);
    DEF_PTR_GETTER_NOTNULL(SDL_Window, window);

    RootRenderEntity *mutable_root() { return &root_; }

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIForm);
private:
    UIFlatMenuGroup *main_menu_ = nullptr;
    UIFlatStatusBar *status_bar_ = nullptr;
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    RootRenderEntity root_;
}; // class UIForm

} // namespace utaha

#endif // UTAHA_UI_FORM_H_
