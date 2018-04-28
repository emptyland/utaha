-- 001
local style = utaha.styleCollection

-- Global Font Setting
style
    :addFont("font.normal", "assets/fonts/default.ttf", 20)
    :addFont("font.small", "assets/fonts/default.ttf", 12)
    :addFont("font.big", "assets/fonts/default.ttf", 20)

-- Flat Menu Setting
style
    :setFont("FlatMenu.font",          "font.normal")
    :setColor("FlatMenu.bg.color",     0x00000080)
    :setColor("FlatMenu.hot.color",    0x007bff00)
    :setColor("FlatMenu.font.color",   0x007bff00)
    :setColor("FlatMenu.border.color", 0xffffff00)
    :setInt("FlatMenu.padding.size",   2)

-- Flat Menu Group Setting
style
    :setFont("FlatMenuGroup.font",          "font.normal")
    :setColor("FlatMenuGroup.bg.color",     0x00000080)
    :setColor("FlatMenuGroup.hot.color",    0x007bff00)
    :setColor("FlatMenuGroup.font.color",   0x007bff00)
    :setColor("FlatMenuGroup.border.color", 0xffffff00)
    :setInt("FlatMenuGroup.padding.h.size", 10)
    :setInt("FlatMenuGroup.padding.v.size", 2)