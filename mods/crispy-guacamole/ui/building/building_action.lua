local EntityData = require 'mods/crispy-guacamole/scripts/entitydata'
local Action = require 'mods/crispy-guacamole/ui/buttons_action/action'

local BuildingAction = {}
BuildingAction.__index = BuildingAction

function BuildingAction:new(parent, building, buildingData)
    local o = setmetatable({
        widgets = nil,
        animationTimer = nil
    }, self)
    o:build(parent, building, buildingData)
    return o
end

function BuildingAction:build(parent, building, buildingData)
    if buildingData and buildingData.units then
        local buttons = {{'A', Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP},
                         {'B', Widget.PositionPolicy.LEFT + Widget.PositionPolicy.CENTER_Y},
                         {'X', Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.CENTER_Y},
                         {'Y', Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM}}
        local widgets = {}
        for i = 1, #buildingData.units do
            local unit = buildingData.units[i]
            local unitAsset = assert(Asset.findFromName('entity', unit.entity), 'Could not find entity asset ' .. unit.entity)
            local unitTemplate = unitAsset:getPath()
            local unitData = assert(EntityData.get(unitTemplate), 'Could not find unit data for ' .. unitTemplate)
            do
                local button = Action:new(buttons[i][1], unit, Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP, building, parent)
                button.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
                widgets[#widgets+1] = button
            end
        end
        self.widgets = widgets
        self:animateWidgets()
    end
end

function BuildingAction:animateWidgets()
    local buttonA, buttonB, buttonX, buttonY = table.unpack(self.widgets)
    self.animationTimer = game.Timer()
    local duration = 0.3
    local easing = flat.easing.easeOut(flat.easing.back)
    local destination = 130
    local origin = 0
    self.animationTimer:onUpdate(function(timer, elapsedTime)
        local t = elapsedTime / duration
        local result = flat.easing.lerp(t, origin, destination, easing)
        buttonA.container:setPosition(0, -result)
        buttonB.container:setPosition(result, 0)
        buttonX.container:setPosition(-result, 0)
        buttonY.container:setPosition(0, result)
    end)
    self.animationTimer:onEnd(function()
        self.animationTimer = nil
    end)
    self.animationTimer:start(duration, false)
end

return BuildingAction