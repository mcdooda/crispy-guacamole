local Theme = require 'mods/dragons-lair/maps/dragon\'s lair/theme'
local Preview = require 'data/scripts/preview'
local Money = require 'mods/dragons-lair/maps/dragon\'s lair/money'

local BuildingIcon = {}
BuildingIcon.__index = BuildingIcon

function BuildingIcon:new(building, parent)
    local o = setmetatable({
        building = building,
        container = nil
    }, self)
    o:build(parent)
    return o
end

function BuildingIcon:build(parent)
    local previewContainer = Widget.makeColumnFlow()
    previewContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
    do
        local preview = Preview.entity(self.building.template)
        preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
        previewContainer:addChild(preview)
    end

    do
        local tooltip = Widget.makeColumnFlow()
        tooltip:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
        tooltip:setBackgroundColor(Theme.BACKGROUND_COLOR)
        tooltip:setPadding(5)
        
        do
            local tooltipLabel = Widget.makeText(self.building.name, table.unpack(Theme.defaultFont))
            tooltipLabel:setTextColor(Theme.TEXT_COLOR)
            tooltip:addChild(tooltipLabel)
        end

        do
            local tooltipLabel = Widget.makeText('Cost: ' .. self.building.cost, table.unpack(Theme.defaultFont))
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
                local buildableZone = Map.getZone 'Buildable'
                Game.setGhostEntity(
                    self.building.template,
                    function(tiles)
                        if Money:getAmount() < self.building.cost then
                            return false
                        end

                        local isInsideBuildableZone = true
                        tiles:eachTile(function(tile)
                            if not buildableZone:isTileInside(tile) then
                                isInsideBuildableZone = false
                            end
                        end)
                        return isInsideBuildableZone
                    end,
                    function()
                        Money:sub(self.building.cost)
                        return true
                    end
                )
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