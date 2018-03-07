local UiSettings = require 'data/scripts/ui/uisettings'
local Path       = require 'data/scripts/path'

return setmetatable(
    {
    	GAME_FONT        = {Path.getModFilePath 'ui/fonts/04B_03__.TTF', 20},
    	DIALOG_COLOR 	 = 0xEDC8ADFF
        BACKGROUND_COLOR = 0x443322CC,
        TEXT_COLOR       = 0xFFFFFFFF
    },
    { __index = UiSettings }
)