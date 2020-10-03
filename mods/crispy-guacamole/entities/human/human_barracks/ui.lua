local HealthBar = require 'mods/crispy-guacamole/ui/entity/healthbar_long'
local ProgressBar = require 'mods/crispy-guacamole/ui/entity/progressbar_long'
local Action = require 'mods/crispy-guacamole/ui/buttons_action/action'
local ui = setmetatable({}, { __index = HealthBar })

local unitData = {
    {
        template = Mod.getFilePath('entities/human/human_soldier'),
        type = 'sword',
        price = 10
    },
    {
        template = Mod.getFilePath('entities/human/human_archer'),
        type = 'bow',
        price = 20
    },
    {
        template = Mod.getFilePath('entities/human/human_crossbowman'),
        type = 'bow',
        price = 70
    },
    {
        template = Mod.getFilePath('entities/human/human_paladin'),
        type = 'wooden_shield',
        price = 120
    }
}

local duration = 0.3
local easing = flat.easing.easeOut(flat.easing.back)
local currentTimer

local function animate(buttonA, buttonB, buttonX, buttonY)
    currentTimer = game.Timer()
    local destination = 130
    local origin = 0
    currentTimer:onUpdate(function(timer, elapsedTime)
        local t = elapsedTime / duration
        local result = flat.easing.lerp(t, origin, destination, easing)
        buttonA.container:setPosition(0, -result)
        buttonB.container:setPosition(result, 0)
        buttonX.container:setPosition(-result, 0)
        buttonY.container:setPosition(0, result)
    end)
    currentTimer:onEnd(function()
        currentTimer = nil
    end)
    currentTimer:start(duration, false)
end

function ui.addedToMap(entity, widget)
    --HealthBar.addedToMap(entity, widget, 34)
    --ProgressBar.addedToMap(entity, widget, 80)

    local container = Widget.makeFixedSize(400, 500)
	local buttonA = Action:new('A', unitData[1], Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP, container)
	local buttonB = Action:new('B', unitData[2], Widget.PositionPolicy.LEFT + Widget.PositionPolicy.CENTER_Y, container)
	local buttonX = Action:new('X', unitData[3], Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.CENTER_Y, container)
	local buttonY = Action:new('Y', unitData[4], Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM, container)
	buttonA.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
	buttonB.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
    buttonX.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
    buttonY.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
	local width, height = container:getSize()
	entity:setUiOffset(flat.Vector2(-width / 2, -height / 2 + 50))
    widget:addChild(container)

    Widget.getRoot():addChild(widget)
    entity:selected(function(entity)
        entity:setUiVisible(true)
        animate(buttonA, buttonB, buttonX, buttonY)
	end)

	entity:deselected(function(entity)
		entity:setUiVisible(false)
    end)
    entity:setUiVisible(false)
end

return ui