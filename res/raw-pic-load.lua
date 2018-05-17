--006
-- require "lfs"

local raw = utaha.rawPicCollection

raw:setDir "tests/img"

raw:addFile "mon1.png"
raw:addFile "mon2.png"
raw:addFileWithMask("mon3.png", 0x52a50800)
raw:addFileWithMask("tile1.png", 0x00757500)
