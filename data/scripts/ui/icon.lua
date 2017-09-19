local UiSettings = require 'data/scripts/ui/uisettings'

local Icon = {}
Icon.__index = Icon

function Icon:new(icon, size)
    size = size or 14
    local container
    if UiSettings.customIcons[icon] then
        container = Widget.makeText(UiSettings.customIcons[icon], UiSettings.customFont, size)
    else
        container = Widget.makeText(UiSettings.icons[icon], UiSettings.glyphFont, size)
    end
    local o = setmetatable({
        container = container
    }, self)
    return o
end

return Icon