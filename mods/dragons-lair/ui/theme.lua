local UiSettings = require 'data/scripts/ui/uisettings'
local Path       = require 'data/scripts/path'

return setmetatable(
    {
        UI_FONT                 = {Path.getModFilePath 'ui/fonts/04B_03__.TTF', 20},
        DIALOG_FONT             = {Path.getModFilePath 'ui/fonts/zeldadxt.ttf', 20},
        DIALOG_TEXT_COLOR       = 0x000000FF,
        DIALOG_BACKGROUND_COLOR = 0xEDC8ADFF,
        BACKGROUND_COLOR        = 0x443322CC,
        TEXT_COLOR              = 0xFFFFFFFF
    },
    { __index = UiSettings }
)