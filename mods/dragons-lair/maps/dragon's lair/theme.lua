local UiSettings = require 'data/scripts/ui/uisettings'
local Path  = require 'data/scripts/path'

return setmetatable(
    {
    	gameFont = {Path.getModFilePath 'ui/fonts/04B_03__.TTF', 20},
        BACKGROUND_COLOR = 0xFFFFFFCC,
        TEXT_COLOR       = 0xFFFFFFFF
    },
    { __index = UiSettings }
)