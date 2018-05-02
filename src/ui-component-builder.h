#ifndef UTAHA_UI_COMPONENT_BUILDER_H_
#define UTAHA_UI_COMPONENT_BUILDER_H_

#include "base.h"
#include "glog/logging.h"
#include <string>

typedef struct lua_State lua_State;

namespace utaha {

class UIComponentFactory;

class UIFlatMenuGroupBuilder;
class UIFlatMenuGroupColumnBuilder;
class UIFlatMenuBuilder;
class UILayoutBuilder;
class UILayoutRowBuilder;
class UIFlatMenuGroup;
class UIFlatMenu;
class UILayout;

class UIComponentBuilder {
public:
    UIComponentBuilder(UIComponentFactory *factory);
    ~UIComponentBuilder();

    UIFlatMenuGroupBuilder *BeginFlatMenuGroup(const char *name);
    UIFlatMenuBuilder *BeginFlatMenu(const char *name);
    UILayoutBuilder *BeginLayout();

    static int BindTo(lua_State *L);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIComponentBuilder);
private:
    UIComponentFactory *factory_;
}; // class UIComponentBuilder


template<class T>
class UIComponentBuilderBase {
public:
    UIComponentBuilderBase(T *component, UIComponentFactory *factory)
        : component_(DCHECK_NOTNULL(component))
        , factory_(DCHECK_NOTNULL(factory)) {}
    inline ~UIComponentBuilderBase() = default;

    DEF_PTR_GETTER_NOTNULL(T, component);
    DEF_PTR_GETTER_NOTNULL(UIComponentFactory, factory);

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIComponentBuilderBase);
private:
    T *component_;
    UIComponentFactory *factory_;
};

class UIFlatMenuGroupBuilder : public UIComponentBuilderBase<UIFlatMenuGroup> {
public:
    inline UIFlatMenuGroupBuilder(UIFlatMenuGroup *component, UIComponentFactory *factory)
        : UIComponentBuilderBase(component, factory) {}
    inline ~UIFlatMenuGroupBuilder() = default;

    UIFlatMenuGroupColumnBuilder *BeginColumn(const char *name, int cmd_id);
    UIFlatMenuGroup *EndMenuGroup();

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatMenuGroupBuilder);
}; // class UIFlatMenuGroupBuilder


class UIFlatMenuGroupColumnBuilder : public UIComponentBuilderBase<UIFlatMenu> {
public:
    UIFlatMenuGroupColumnBuilder(const char *name, int cmd_id, void *param,
                                 UIFlatMenuGroupBuilder *builder);
    inline UIFlatMenuGroupColumnBuilder() = default;

    UIFlatMenuGroupColumnBuilder *AddItem(const char *name, int cmd_id, void *param);
    UIFlatMenuGroupColumnBuilder *AddDiv();
    UIFlatMenuGroupBuilder *EndColumn();

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatMenuGroupColumnBuilder);
private:
    std::string col_name_;
    int         col_cmd_id_;
    void       *col_param_;
    UIFlatMenuGroupBuilder *builder_;
}; // class UIFlatMenuGroupColumnBuilder

class UIFlatMenuBuilder : public UIComponentBuilderBase<UIFlatMenu> {
public:
    inline UIFlatMenuBuilder(UIFlatMenu *component, UIComponentFactory *factory)
        : UIComponentBuilderBase(component, factory) {}
    inline ~UIFlatMenuBuilder() = default;

    UIFlatMenuBuilder *AddItem(const char *name, int cmd_id, void *param);
    UIFlatMenuBuilder *AddDiv();
    UIFlatMenu *EndMenu();

    DISALLOW_IMPLICIT_CONSTRUCTORS(UIFlatMenuBuilder);
}; // class UIFlatMenuBuilder


class UILayoutBuilder {
public:
    inline UILayoutBuilder(UILayout *layout) : layout_(DCHECK_NOTNULL(layout)) {}
    inline ~UILayoutBuilder() = default;

    UILayoutBuilder *LetPaddingSize(int padding_size);
    UILayoutBuilder *LetVerticalAlignment(int aligment);
    UILayoutBuilder *LetHorizontalAligment(int aligment);
    UILayoutBuilder *LetDebugMode(bool turn_on);
    UILayoutRowBuilder *BeginRow(int aligment);
    UILayout *EndLayout();

    DISALLOW_IMPLICIT_CONSTRUCTORS(UILayoutBuilder);
private:
    UILayout *layout_;
}; // class UILayoutBuilder

} // namespace utaha

#endif // UTAHA_UI_COMPONENT_BUILDER_H_
