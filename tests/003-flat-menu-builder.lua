--003

local builder = utaha.uiBuilder
local R = utaha.R

return builder:beginFlatMenu("test")
    :addItem("test.1", R.ID_TEST_1, nil) -- 0
    :addDiv()                            -- 1
    :addItem("test.2", R.ID_TEST_2, nil) -- 2
    :addDiv()                            -- 3
    :addItem("test.3", R.ID_TEST_3, nil) -- 4
:endMenu()
