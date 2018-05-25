--006
-- require "lfs"

local raw = utaha.rawPicCollection

raw:setDir "tests/img"

raw:addFileWithMask("man1.png", 0x00757500)
raw:addFileWithMask("mon1.png", 0x52a50800)
raw:addFileWithMask("mon2.png", 0x21940000)
raw:addFileWithMask("mon3.png", 0x52a50800)
raw:addFileWithMask("tile1.png", 0x00757500)
