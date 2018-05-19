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

local statusBar = builder:beginFlatStatusBar("main-status-bar")
    :h(22)
    :addGrid(70)
        :text("Ready") -- [0]
    :endGrid()
    :addGrid(120)
        :text("----") -- [1]
    :endGrid()
    :addGrid(120)
        :text("----") -- [2]
    :endGrid()    
:endStatusBar()

local picGridSelector = builder:beginPicGridSelector("grid-selector")
    :gridSizeW(48)
    :gridSizeH(64)
:endPicGridSelector()

local gridSizeWInputBox = builder:beginFlatInputBox("grid-size-w-input")
    :text(48)
    :maxInput(100)
    :h(22)
    :w(60)
:endInputBox()

local gridSizeHInputBox = builder:beginFlatInputBox("grid-size-h-input")
    :text(64)
    :maxInput(100)
    :h(22)
    :w(60)
:endInputBox()

local rightLayout = builder:beginLayout()
    :paddingSize(1)
    :verticalAlignment(alignment.ALIGN_LEFT_OR_TOP)
    :horizontalAligment(alignment.ALIGN_RIGHT_OR_BOTTOM)
    :beginRow(alignment.ALIGN_LEFT_OR_TOP)
        :add(gridSizeWInputBox)
        :add(gridSizeHInputBox)
    :endRow()
    :beginRow(alignment.ALIGN_LEFT_OR_TOP)
        :add(picGridSelector)
    :endRow()
:endLayout()

return {
    mainMenu = mainMenu,
    statusBar = statusBar,
    rightLayout = rightLayout
}