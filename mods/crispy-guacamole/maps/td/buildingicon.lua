local Theme = require 'mods/crispy-guacamole/maps/td/theme'
local Preview = require 'data/scripts/preview'

local BuildingIcon = {}
BuildingIcon.__index = BuildingIcon

function BuildingIcon:new(entityTemplateName, parent)
    local o = setmetatable({
        entityTemplateName = entityTemplateName,
        container = nil
    }, self)
    o:build(parent)
    return o
end

function BuildingIcon:build(parent)
    local previewContainer = Widget.makeColumnFlow()
    previewContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
    do
        local preview = Preview.entity(self.entityTemplateName)
        preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
        previewContainer:addChild(preview)
    end

    do
        local tooltip = Widget.makeColumnFlow()
        tooltip:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
        tooltip:setBackgroundColor(Theme.BACKGROUND_COLOR)
        
        do
            local tooltipLabel = Widget.makeText(self.entityTemplateName, table.unpack(Theme.defaultFont))
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

            previewContainer:click(function()
                print(self.entityTemplateName)
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

return BuildingIcon