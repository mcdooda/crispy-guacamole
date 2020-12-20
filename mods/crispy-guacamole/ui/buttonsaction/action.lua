local Button = require 'mods/crispy-guacamole/ui/buttonsaction/button'
local Path  = require 'data/scripts/path'
local Theme = require 'mods/crispy-guacamole/ui/theme'
local UiSettings = require 'data/scripts/ui/uisettings'
local Preview = require 'data/scripts/preview'
local BuildEntity = require 'mods/crispy-guacamole/ui/building/buildentity'

local frameHeight = 144
local frameWidth = 111
local infoHeight = 33
local uiRadius = 350

local Action = {}
Action.__index = Action

function Action:new(buttonType, unit, buttonPositionPolicy, building, parent)
    local o = setmetatable({
        unit = unit,
        button = nil,
        container = nil,
        animationTimer = nil,
        building = building
    }, self)
    o:build(parent, buttonType, buttonPositionPolicy)
    return o
end

function Action:validateAnimation(buttonPositionPolicy)
    local externalBorder = Widget.makeImage(Path.getModFilePath 'ui/buttonsaction/validate.png')
    externalBorder:setPositionPolicy(Widget.PositionPolicy.CENTER)
    externalBorder:setSize(frameWidth, frameHeight)

    local internalColor = Widget.makeFixedSize(frameWidth, frameHeight)
    internalColor:setPositionPolicy(Widget.PositionPolicy.CENTER)
    internalColor:setBackgroundColor(0xFFF894FF)
    self.container:addChild(externalBorder)
    self.container:addChild(internalColor)

    do
        self.animationTimer = game.Timer()
        local destination = 1
        local origin = 0
        local duration = 1
        local width, height = externalBorder:getSize()
        self.animationTimer:onUpdate(function(timer, elapsedTime)
            local t = elapsedTime / duration
            local result = flat.easing.lerp(t, origin, destination, flat.easing.easeOut(flat.easing.cubic))
            externalBorder:setSize(width * (1 + result), height * (1 + result))
            local alpha = math.floor(0xFF * (1 - result))
            internalColor:setBackgroundColor( 0xFFF89400 + alpha)
        end)
        self.animationTimer:onEnd(function()
            self.animationTimer = nil
            self.container:removeChild(externalBorder)
            self.container:removeChild(internalColor)
        end)
        self.animationTimer:start(duration, false)
    end
end

function Action:build(parent, buttonType, buttonPositionPolicy)
    local container = Widget.makeFixedSize(150, 210)

    container:gamepadButtonPressed(Gamepads.GamepadButton[buttonType], function()
        if self.building:isSelected() then
            self:validateAnimation(buttonPositionPolicy)
            self.button:setPressed(true)
            BuildEntity.addToQueue(self.building, self.unit)
        end
    end)
    container:gamepadButtonReleased(Gamepads.GamepadButton[buttonType], function()
        self.button:setPressed(false)
    end)

    local background = Widget.makeImage(Path.getModFilePath 'ui/buttonsaction/background.png')
    do
        background:setPositionPolicy(Widget.PositionPolicy.CENTER)
        background:setSize(frameWidth, frameHeight)
        container:addChild(background)
    end
    do
        local priceContainer = Widget.makeFixedSize(frameWidth, infoHeight)
        priceContainer:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM)
        do
            local priceLabel = Widget.makeText(tostring(10), table.unpack(UiSettings.defaultFont), 25)
            priceLabel:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.TOP)
            priceLabel:setPosition(-30, 0)
            priceLabel:setTextColor(0xFFFFFFFF)
            priceContainer:addChild(priceLabel)
        end

        do
            local currencyIcon = Widget.makeImage(Path.getModFilePath 'ui/buttonsaction/coin.png')
            currencyIcon:setSize(21, 21)
            currencyIcon:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.CENTER_Y)
            currencyIcon:setPosition(-3, 0)
            priceContainer:addChild(currencyIcon)
        end
        background:addChild(priceContainer)
    end

    do
        local typeIcon = Widget.makeImage(Path.getModFilePath('ui/buttonsaction/typeicon/' .. 'sword' .. '.png'))
        typeIcon:setPosition(10, -frameHeight / 2)
        typeIcon:setSize(30, 30)
        background:addChild(typeIcon)
    end

    do
        local unitAsset = assert(Asset.findFromName('entity', self.unit.entity), 'Could not find entity asset ' .. self.unit.entity)
        local unitTemplate = unitAsset:getPath()
        local preview = Preview.entity(unitTemplate, 'move', true, 3)
        preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
        local template = Path.requireComponentTemplate(unitTemplate, 'sprite')
        preview:setPosition(template.origin:x(), template.origin:y())
        background:addChild(preview)
    end

    do
        self.button = Button:new(container, buttonType, false)
        self.button.container:setPositionPolicy(buttonPositionPolicy)
    end

    parent:addChild(container)
    self.container = container
end

return Action