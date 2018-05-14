#include "ui-form.h"
#include "ui-style-collection.h"
#include "ui-component-factory.h"
#include "ui-component-builder.h"
#include "glog/logging.h"

namespace utaha {

class EditorForm : public UIForm {
public:
    EditorForm();
    virtual ~EditorForm();

    virtual int OnCommand(UIComponent *sender, int cmd_id, void *param,
                          bool *is_break) override;

    virtual int OnInit() override;
//    virtual void OnEvent(SDL_Event *e) override;
//    virtual void OnAfterRender() override;
//    virtual void OnBeforeRender() override;
    virtual void OnQuit() override;

    DISALLOW_IMPLICIT_CONSTRUCTORS(EditorForm);
private:
    UIStyleCollection *styles_ = nullptr;
    UIComponentFactory *factory_ = nullptr;
}; // class EditorForm

UIForm *CreateEditorForm() {
    return new EditorForm();
}

EditorForm::EditorForm() {
}

/*virtual*/ EditorForm::~EditorForm() {
}

/*virtual*/ int EditorForm::OnCommand(UIComponent *sender, int cmd_id, void *param,
                                      bool *is_break) {

    return 0;
}

/*virtual*/ int EditorForm::OnInit() {
    styles_ = new UIStyleCollection();
    auto err = styles_->LoadFromFile("res/styles.lua");
    if (err) {
        LOG(ERROR) << "Can not load styles file. " << err;
        return -1;
    }
    factory_ = CreateUIComponentStyleFactory(styles_);
    if (!factory_) {
        LOG(ERROR) << "Can not create ui-component-factory. ";
        return -1;
    }

    //UIComponentBuilder builder(factory_, this, this);

    return UIForm::OnInit();
}

/*virtual*/ void EditorForm::OnQuit() {
    delete factory_; factory_ = nullptr;
    delete styles_; styles_ = nullptr;
}

} // namespace utaha
