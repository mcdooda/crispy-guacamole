local Theme   = require 'mods/dragons-lair/ui/theme'
local EntityPreview = require 'mods/dragons-lair/scripts/entitypreview'

local ItemIcon = {}
ItemIcon.__index = ItemIcon

function ItemIcon:new(item, parent)
    local o = setmetatable({
        item = item,
        preview = nil
    }, self)
    o:build(parent)
    return o
end

function ItemIcon:build(parent)
    local preview = EntityPreview:new('items', 3)
    preview:startAnimation(self.item.name)
    preview.widget:setPositionPolicy(Widget.PositionPolicy.CENTER)
    do
        local tooltip = Widget.makeColumnFlow()
        tooltip:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
        tooltip:setBackgroundColor(Theme.BACKGROUND_COLOR)
        tooltip:setPadding(5)
        
        do
            local tooltipLabel = Widget.makeText(self.item.name, table.unpack(Theme.defaultFont))
            tooltipLabel:setTextColor(Theme.TEXT_COLOR)
            tooltip:addChild(tooltipLabel)
        end

        do
            local tooltipLabel = Widget.makeText(self.item.description, table.unpack(Theme.defaultFont))
            tooltipLabel:setTextColor(Theme.TEXT_COLOR)
            tooltip:addChild(tooltipLabel)
        end

        do
            preview.widget:mouseEnter(function()
                Widget.getRoot():addChild(tooltip)
            end)

            preview.widget:mouseLeave(function()
                tooltip:removeFromParent()
            end)

            preview.widget:mouseMove(function()
                local x, y = Mouse.getPosition()
                local w, h = tooltip:getComputedSize()
                tooltip:setPosition(x - w - 5, y - h / 2)
            end)
        end
    end

    parent:addChild(preview.widget)
    self.preview = preview
end

return ItemIcon