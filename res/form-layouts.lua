local builder = utaha.uiBuilder
local alignment = utaha.alignment
local R = utaha.R

local mainMenu = builder:beginFlatMenuGroup("main-menu")
    :beginColumn("File", 1)
        :addItem("Tile", R.ID_FILE_TILE, nil)
        :addItem("Spirit", R.ID_FILE_SPIRIT, nil)
        :addItem("Map", R.ID_FILE_MAP, nil)
        :addDiv()
        :addItem("Save All", R.ID_FILE_SAVE_ALL, nil)
    :endColumn()
    :beginColumn("Tile", 2)
        :addItem("New Tile", R.ID_TILE_NEW, nil)
        :addItem("Commit Tile", R.ID_TILE_COMMIT, nil)
        :addDiv()
        :addItem("Next Tile", R.ID_TILE_NEXT, nil)
        :addItem("Prev Tile", R.ID_TILE_PREV, nil)
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

--------------------------------------------------------------------------------

local picGridSelector = builder:beginPicGridSelector("grid-selector")
    :gridSizeW(48)
    :gridSizeH(64)
:endPicGridSelector()

local gridSizeWInputBox = builder:beginFlatInputBox("grid-size-w-input")
    :text(48)
    :maxInput(100)
    :h(26)
    :w(60)
:endInputBox()

local gridSizeHInputBox = builder:beginFlatInputBox("grid-size-h-input")
    :text(64)
    :maxInput(100)
    :h(26)
    :w(60)
:endInputBox()

local rightLayout = builder:beginLayout()
    :paddingSize(1)
    :verticalAlignment(alignment.ALIGN_LEFT_OR_TOP)
    :horizontalAligment(alignment.ALIGN_RIGHT_OR_BOTTOM)
    :beginRow(alignment.ALIGN_LEFT_OR_TOP)
        :add(builder:flatLabel("l", "W:"))
        :add(gridSizeWInputBox)
        :add(builder:flatLabel("l", "H:"))
        :add(gridSizeHInputBox)
    :endRow()
    :beginRow(alignment.ALIGN_LEFT_OR_TOP)
        :add(picGridSelector)
    :endRow()
:endLayout()

--------------------------------------------------------------------------------

local tileId = builder:beginFlatInputBox("tile-id")
    :text('0')
    :maxInput(100)
    :h(26)
    :w(60)
:endInputBox()

-- clipping
local tileClipping = builder:beginFlatInputBox("tile-clipping")
    :text('0')
    :maxInput(100)
    :h(26)
    :w(60)
:endInputBox()

local tileLayout = builder:beginLayout()
    :paddingSize(1)
    :verticalAlignment(alignment.ALIGN_LEFT_OR_TOP)
    :horizontalAligment(alignment.ALIGN_LEFT_OR_TOP)
    :beginRow(alignment.ALIGN_LEFT_OR_TOP)
        :add(builder:flatLabel("l", "Tile ID:"))
        :add(tileId)
        :add(builder:flatLabel("l", "Clipping: "))
        :add(tileClipping)
    :endRow()
    :beginRow(alignment.ALIGN_LEFT_OR_TOP)
        :add(builder:flatLabel("l", "Pass Mask:"))
        :add(builder:beginFlatCheckBox("walk-pass")
            :label("Walk")
            :value(true)
        :endCheckBox())
        :add(builder:beginFlatCheckBox("bullet-pass")
            :label("Bullet")
            :value(true)
        :endCheckBox())
        :add(builder:beginFlatCheckBox("magic-pass")
            :label("Magic")
            :value(true)
        :endCheckBox())
        :add(builder:beginFlatCheckBox("fly-pass")
            :label("Fly")
            :value(true)
        :endCheckBox())
    :endRow()
:endLayout()

return {
    mainMenu = mainMenu,
    statusBar = statusBar,
    rightLayout = rightLayout,
    tileLayout = tileLayout
}