local builder = utaha.uiBuilder
local alignment = utaha.alignment
local R = utaha.R

local mainMenu = builder:beginFlatMenuGroup("main-menu")
    :beginColumn("文件", 1)
        :addItem("建筑", R.ID_EDIT_CONSTRUCTION, nil)
        :addItem("精灵", R.ID_EDIT_SPIRIT, nil)
        :addItem("地图", R.ID_EDIT_MAP, nil)
        :addDiv()
        :addItem("保存所有", R.ID_FILE_SAVE_ALL, nil)
    :endColumn()
:endMenuGroup()

return { mainMenu = mainMenu }