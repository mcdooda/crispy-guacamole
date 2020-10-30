local EntityData = require 'mods/crispy-guacamole/scripts/entitydata'
local Action = require 'mods/crispy-guacamole/ui/buttons_action/action'

local animationTimer

local function animateWidgets(buttonA, buttonB, buttonX, buttonY)
    animationTimer = game.Timer()
    local duration = 0.3
    local easing = flat.easing.easeOut(flat.easing.back)
    local destination = 130
    local origin = 0
    animationTimer:onUpdate(function(timer, elapsedTime)
        local t = elapsedTime / duration
        local result = flat.easing.lerp(t, origin, destination, easing)
        buttonA.container:setPosition(0, -result)
        buttonB.container:setPosition(result, 0)
        buttonX.container:setPosition(-result, 0)
        buttonY.container:setPosition(0, result)
    end)
    animationTimer:onEnd(function()
        animationTimer = nil
    end)
    animationTimer:start(duration, false)
end


local function getUnitData(unit)
    return assert(EntityData.get(entityAsset:getPath()), 'Could not find unit data for ' .. entityAsset:getPath())
end

local function buildWidgets(parent, building, buildingData)
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
                --[[label:click(function()
                    for i = 1, #buildings do
                        addToQueue(buildings[i], unit)
                    end
                end)]]
                widgets[#widgets+1] = button
            end
        end
        animateWidgets(table.unpack(widgets))
    end
end

return {
    buildWidgets = buildWidgets,
    animateWidgets = animateWidgets
}