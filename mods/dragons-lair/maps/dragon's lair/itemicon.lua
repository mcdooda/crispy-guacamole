local Theme   = require 'mods/dragons-lair/maps/dragon\'s lair/theme'
local Preview = require 'data/scripts/preview'
local Money   = require 'mods/dragons-lair/maps/dragon\'s lair/money'

local ItemIcon = {}
ItemIcon.__index = ItemIcon

function ItemIcon:new(item, parent)
    local o = setmetatable({
        item = item,
        container = nil
    }, self)
    o:build(parent)
    return o
end

function ItemIcon:build(parent)
    local previewContainer = Widget.makeColumnFlow()
    previewContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.EXPAND_Y)
    do
        local preview = Preview.entity('items', self.item.name, 1, 3)
        preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
        previewContainer:addChild(preview)
    end

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
            local tooltipLabel = Widget.makeText('Cost: ' .. self.item.cost, table.unpack(Theme.defaultFont))
            tooltipLabel:setTextColor(Theme.TEXT_COLOR)
            tooltip:addChild(tooltipLabel)
        end

        do
            previewContainer:mouseEnter(function()
                Widget.getRoot():addChild(tooltip)
            end)

            previewContainer:mouseLeave(function()
                tooltip:removeFromParent()
            end)

            previewContainer:mouseMove(function()
                local x, y = Mouse.getPosition()
                local w, h = tooltip:getComputedSize()
                tooltip:setPosition(x - w - 5, y - h / 2)
            end)
        end
    end

    parent:addChild(previewContainer)
    self.container = previewContainer
end

return ItemIcon