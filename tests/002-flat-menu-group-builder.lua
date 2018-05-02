--001

local builder = utaha.uiBuilder

local mg1 = builder:beginFlatMenuGroup("test")
    :beginColumn("test.column.1", 1)
        :addItem("test.1", 2, nil)
        :addDiv()
        :addItem("test.2", 3, nil)
    :endColumn()
:endMenuGroup()

local n = 1 + 1

return mg1