-- 001
local style = utaha.styleCollection

-- Global Font Styles
style
    :addFont("font.normal", "assets/fonts/default.ttf", 18)
    :addFont("font.small", "assets/fonts/default.ttf", 12)
    :addFont("font.big", "assets/fonts/default.ttf", 20)

-- Flat Menu Styles
    :setFont("FlatMenu.font",          "font.normal")
    :setColor("FlatMenu.bg.color",     0x00000080)
    :setColor("FlatMenu.hot.color",    0x007bff00)
    :setColor("FlatMenu.font.color",   0xffffff00)
    :setColor("FlatMenu.border.color", 0xffffff00)
    :setInt("FlatMenu.padding.size",   4)

-- Flat Menu Group Styles
    :setFont("FlatMenuGroup.font",          "font.normal")
    :setColor("FlatMenuGroup.bg.color",     0x00000080)
    :setColor("FlatMenuGroup.hot.color",    0x007bff00)
    :setColor("FlatMenuGroup.font.color",   0xffffff00)
    :setColor("FlatMenuGroup.border.color", 0xffffff00)
    :setInt("FlatMenuGroup.padding.h.size", 10)
    :setInt("FlatMenuGroup.padding.v.size", 2)

-- Flat Button Styles
    :setFont("FlatButton.font", "font.normal")
    :setSize("FlatButton.defaultSize",    40, 22)
    :setColor("FlatButton.bg.color",      0x007bff00)
    :setColor("FlatButton.hot.color",     0x108bff00)
    :setColor("FlatButton.font.color",    0xffffff00)
    :setColor("FlatButton.pressed.color", 0x007bff00)

-- Flat Input Box Styles
    :setFont("FlatInputBox.font", "font.normal")
    :setSize("FlatInputBox.defaultSize",    40, 22)
    :setInt("FlatInputBox.defaultMaxInput", 32)
    :setColor("FlatInputBox.bg.color",      0x00000080)
    :setColor("FlatInputBox.font.color",    0x00ff0000)
    :setColor("FlatInputBox.border.color",  0xffffff00)
    :setInt("FlatInputBox.padding.size",    4)
