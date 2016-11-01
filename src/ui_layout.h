#ifndef UTAHA_UI_LAYOUT_H_
#define UTAHA_UI_LAYOUT_H_

namespace utaha {

class UILayout {
public:
    UILayout(int id);

    virtual ~UILayout();

    virtual void OnKeyInput(int key, int code, int action,
                            int mods) = 0;

    virtual void OnMouseInput(int button, int action, int mods) = 0;

    virtual void OnDraw() = 0;

    int id() const { return id_; }

private:
    int id_ = -1;

};

} // namespace utaha

#endif // UTAHA_UI_LAYOUT_H_
