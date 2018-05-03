-- 004

local builder = utaha.uiBuilder
local R = utaha.R
local alignment = utaha.alignment

return builder:beginLayout()
    :paddingSize(1)
    :verticalAlignment(alignment.ALIGN_CENTER)
    :horizontalAligment(alignment.ALIGN_CENTER)
    :debugMode(true)
    :beginRow(alignment.ALIGN_CENTER)
        :add(builder:beginFlatMenu("test")
            :addItem("test.1", 100, nil) -- 0
            :addDiv()                            -- 1
            :addItem("test.2", 200, nil) -- 2
            :addDiv()                            -- 3
            :addItem("test.3", 300, nil) -- 4
        :endMenu())
    :endRow()
:endLayout()