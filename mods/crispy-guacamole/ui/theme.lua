local UiSettings = require 'data/scripts/ui/uisettings'

return setmetatable(
    {
        BACKGROUND_COLOR = 0xFFFFFFCC,
        TEXT_COLOR       = 0x111111FF,

        mineralsFont              = {'data/misc/fonts/Lato-Medium.ttf', 17},
        MINERALS_TEXT_COLOR       = 0x3BE0F9,
        MINERALS_BACKGROUND_COLOR = 0xFFFFFFFF
    },
    { __index = UiSettings }
)