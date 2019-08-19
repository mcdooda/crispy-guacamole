local Theme = require 'mods/crispy-guacamole/ui/theme'
local Preview = require 'data/scripts/preview'
local Money = require 'mods/crispy-guacamole/maps/td/money'

local BuildingIcon = {}
BuildingIcon.__index = BuildingIcon

function BuildingIcon:new(towerData, parent)
    local o = setmetatable({
        towerData = towerData,
        container = nil
    }, self)
    o:build(parent)
    return o
end

function BuildingIcon:build(parent)
    local previewContainer = Widget.makeColumnFlow()
    previewContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
    do
        local preview = Preview.entity(self.towerData.template)
        preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
        previewContainer:addChild(preview)
    end

    do
        local tooltip = Widget.makeColumnFlow()
        tooltip:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
        tooltip:setBackgroundColor(Theme.BACKGROUND_COLOR)
        tooltip:setPadding(5)
        
        do
            local tooltipLabel = Widget.makeText(self.towerData.name, table.unpack(Theme.defaultFont))
            tooltipLabel:setTextColor(Theme.TEXT_COLOR)
            tooltip:addChild(tooltipLabel)
        end

        do
            local tooltipLabel = Widget.makeText('Cost: ' .. self.towerData.cost, table.unpack(Theme.defaultFont))
            tooltipLabel:setTextColor(Theme.TEXT_COLOR)
            tooltip:addChild(tooltipLabel)
        end

        do
            local tooltipLabel = Widget.makeText(self.towerData.description, table.unpack(Theme.defaultFont))
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
                game.setGhostEntity(
                    self.towerData.template,
                    function(tiles)
                        if Money:getAmount() < self.towerData.cost then
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
                    function(tiles)
                        Money:sub(self.towerData.cost)
                        if self.towerData.onPlace then
                            self.towerData.onPlace()
                        end
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