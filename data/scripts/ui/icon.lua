local UiSettings = require 'data/scripts/ui/uisettings'

local Icon = {
    iconFontsPath = 'data/misc/icons'
}

Icon.__index = Icon

function Icon:new(icon, size)
    size = size or 14
    local character, fontPath = self:findIcon(icon)
    local text = Widget.makeText(character, fontPath, size)
    local container = Widget.makeLineFlow()
    container:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
    container:setMargin(0, 3, 0, 3)
    container:addChild(text)
    local o = setmetatable({
        container = container,
        text = text,
        icon = icon,
        size = size
    }, self)
    return o
end

function Icon:setIcon(icon)
    local character, fontPath = self:findIcon(icon)
    self.icon = icon
    self.text:removeFromParent()
    local text = Widget.makeText(character, fontPath, self.size)
    self.container:addChild(text)
    self.text = text
end

function Icon:setSize(size)
    self.size = size
    self.text:setSize(size)
end

function Icon:setColor(color)
    self.text:setTextColor(color)
end

function Icon:findIcon(icon)
    for i = 1, #UiSettings.fontIcons do
        local fontIcon = UiSettings.fontIcons[i]
        local icons = require(self.iconFontsPath .. '/' .. fontIcon)
        if icons[icon] then
            return icons[icon], self.iconFontsPath .. '/' .. fontIcon .. '.ttf'
        end
    end
    error('no icon for ' .. icon)
end

return Icon