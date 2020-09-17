local Preview = require 'data/scripts/preview'
local Theme = require 'mods/crispy-guacamole/ui/theme'
local EntityData = require 'mods/crispy-guacamole/scripts/entitydata'

local pairs = pairs

local root = Widget.getRoot()

local toprightPanel

local buttonXPicto = 'mods/crispy-guacamole/ui/controls/button_1.png'
local buttonYPicto = 'mods/crispy-guacamole/ui/controls/button_2.png'
local buttonAPicto = 'mods/crispy-guacamole/ui/controls/button_3.png'
local buttonBPicto = 'mods/crispy-guacamole/ui/controls/button_4.png'
local bgpath = 'mods/crispy-guacamole/ui/controls/bg.png'
local function makeButton(parent, path)
    local container = Widget.makeFixedSize(80, 110)
    container:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
    container:setPadding(0)
    container:setMargin(0)
    do
        local bgImage = Widget.makeImage(bgpath)
        bgImage:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.CENTER_Y)
        
        local imageWidth, imageHeight = Image.getSize(bgpath)
        bgImage:setSize(
            imageWidth * 3,
            imageHeight * 3
        )
        container:addChild(bgImage)
    end
    do
        local buttonImage = Widget.makeImage(path)
        buttonImage:setPositionPolicy(Widget.PositionPolicy.BOTTOM + Widget.PositionPolicy.CENTER_X)
        
        local imageWidth, imageHeight = Image.getSize(path)
        buttonImage:setSize(
            imageWidth * 3,
            imageHeight * 3
        )
        container:addChild(buttonImage)
    end

    parent:addChild(container)
    return {
        container    = container
    }
end


local function buildWidgets()
    local container = Widget.makeFixedSize(220, 220)
    container:setPositionPolicy(Widget.PositionPolicy.TOP + Widget.PositionPolicy.LEFT)
    container:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
    container:setPadding(5)
    container:setMargin(20)
    
    local buttonX = makeButton(container, buttonXPicto)
    buttonX.container:setPositionPolicy(Widget.PositionPolicy.LEFT + Widget.PositionPolicy.CENTER_Y)
    local buttonY = makeButton(container, buttonYPicto)
    buttonY.container:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP)
    local buttonB = makeButton(container, buttonBPicto)
    buttonB.container:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.CENTER_Y)
    local buttonA = makeButton(container, buttonAPicto)
    buttonA.container:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM)

    root:addChild(container)
end

return {
    buildWidgets = buildWidgets
}