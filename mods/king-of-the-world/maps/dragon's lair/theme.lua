local UiSettings = require 'data/scripts/ui/uisettings'

return setmetatable(
    {
        BACKGROUND_COLOR = 0xFFFFFFCC,
        TEXT_COLOR       = 0x111111FF
    },
    { __index = UiSettings }
)