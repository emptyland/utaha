-- 005

local builder = utaha.uiBuilder
local alignment = utaha.alignment

return builder:beginLayout()
    :paddingSize(1)
    :verticalAlignment(alignment.ALIGN_LEFT_OR_TOP)
    :horizontalAligment(alignment.ALIGN_LEFT_OR_TOP)
    :debugMode(true)
    :beginRow(alignment.ALIGN_LEFT_OR_TOP)
        :add(builder:beginFlatMenuGroup("test")
            :beginColumn("汉字", 1)
                :addItem("条目1", 2, nil)
                :addDiv()
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
            :endColumn()
            :beginColumn("汉字", 1)
                :addItem("条目1", 2, nil)
                :addDiv()
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
            :endColumn()
            :beginColumn("汉字", 1)
                :addItem("条目1", 2, nil)
                :addDiv()
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
            :endColumn()
            :beginColumn("汉字", 1)
                :addItem("条目1", 2, nil)
                :addDiv()
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
                :addItem("条目2", 3, nil)
            :endColumn()
        :endMenuGroup())
    :endRow()
:endLayout()