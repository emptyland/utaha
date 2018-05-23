#ifndef UTAHA_UI_COMPONENT_FACTORY_H_
#define UTAHA_UI_COMPONENT_FACTORY_H_

#include "base.h"
#include <string>

namespace utaha {

class UIFlatButton;
class UIFlatMenu;
class UIFlatMenuGroup;
class UIFlatInputBox;
class UIFlatLabel;
class UIFlatCheckBox;
class UIFlatStatusBar;
class UIFlatPicView;
class UIPicGridSelector;
class UIStyleCollection;

class UIComponentFactory {
public:
    UIComponentFactory() {}
    virtual ~UIComponentFactory();

    virtual UIFlatButton *CreateFlatButton(const std::string &name) = 0;
    virtual UIFlatMenu *CreateFlatMenu(const std::string &name) = 0;
    virtual UIFlatMenuGroup *CreateFlatMenuGroup(const std::string &name) = 0;
    virtual UIFlatInputBox *CreateFlatInputBox(const std::string &name) = 0;
    virtual UIFlatLabel *CreateFlatLabel(const std::string &name) = 0;
    virtual UIFlatCheckBox *CreateFlatCheckBox(const std::string &name) = 0;
    virtual UIFlatStatusBar *CreateFlatStatusBar(const std::string &name) = 0;
    virtual UIFlatPicView *CreateFlatPicView(const std::string &name) = 0;
    virtual UIPicGridSelector *CreatePicGridSelector(const std::string &name) = 0;

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIComponentFactory);
}; // class UIComponentFactory

UIComponentFactory *CreateUIComponentStyleFactory(UIStyleCollection *style);

} // namespace utaha

#endif // UTAHA_UI_COMPONENT_FACTORY_H_
