-- 001
local style = utaha.styleCollection

-- Global Font Styles
style
    :addFont("font.normal", "res/fonts/wqy-zenhei.ttf", 18)
    :addFont("font.small", "res/fonts/wqy-zenhei.ttf", 12)
    :addFont("font.big", "res/fonts/wqy-zenhei.ttf", 20)

-- Flat Menu Styles
    :setFont("FlatMenu.font",          "font.normal")
    :setColor("FlatMenu.bg.color",     0x000000ff)
    :setColor("FlatMenu.hot.color",    0x007bffff)
    :setColor("FlatMenu.font.color",   0xffffffff)
    :setColor("FlatMenu.border.color", 0xffffffff)
    :setInt("FlatMenu.padding.size",   4)

-- Flat Menu Group Styles
    :setFont("FlatMenuGroup.font",          "font.normal")
    :setColor("FlatMenuGroup.bg.color",     0x000000ff)
    :setColor("FlatMenuGroup.hot.color",    0x007bffff)
    :setColor("FlatMenuGroup.font.color",   0xffffffff)
    :setColor("FlatMenuGroup.border.color", 0xffffffff)
    :setInt("FlatMenuGroup.padding.h.size", 10)
    :setInt("FlatMenuGroup.padding.v.size", 2)

-- Flat Button Styles
    :setFont("FlatButton.font", "font.normal")
    :setSize("FlatButton.defaultSize",    40, 22)
    :setColor("FlatButton.bg.color",      0x007bffff)
    :setColor("FlatButton.hot.color",     0x108bffff)
    :setColor("FlatButton.font.color",    0xffffffff)
    :setColor("FlatButton.pressed.color", 0x007bffff)

-- Flat Input Box Styles
    :setFont("FlatInputBox.font", "font.normal")
    :setSize("FlatInputBox.defaultSize",    40, 22)
    :setInt("FlatInputBox.defaultMaxInput", 32)
    :setColor("FlatInputBox.bg.color",      0x000000ff)
    :setColor("FlatInputBox.font.color",    0x00ff00ff)
    :setColor("FlatInputBox.border.color",  0xffffffff)
    :setInt("FlatInputBox.padding.size",    4)

-- Flat Label Styles
    :setFont("FlatLabel.font",          "font.normal")
    :setColor("FlatLabel.bg.color",     0x0f0f0fff)
    :setColor("FlatLabel.font.color",   0xffffffff)
    :setColor("FlatLabel.border.color", 0x00000000)
    :setInt("FlatLabel.padding.h.size", 10)
    :setInt("FlatLabel.padding.v.size", 2)

-- Flat Check Box Styles
    :setFont("FlatCheckBox.font",          "font.normal")
    :setColor("FlatCheckBox.font.color",   0xffffffff)
    :setColor("FlatCheckBox.box.color",    0x007bffff)
    :setInt("FlatCheckBox.padding.h.size", 10)
    :setInt("FlatCheckBox.padding.v.size", 2)

-- Flat Status Bar Styles
    :setFont("FlatStatusBar.font",          "font.normal")
    :setColor("FlatStatusBar.font.color",   0xffffffff)
    :setColor("FlatStatusBar.bg.color",     0x007bffff)
    :setColor("FlatStatusBar.border.color", 0xffffffff)
    :setInt("FlatStatusBar.padding.size",   10)

-- Pic Grid Selector Styles
    :setColor("PicGridSelector.grid.color",     0x00ff00ff)
    :setColor("PicGridSelector.selected.color", 0x80ff008f)
