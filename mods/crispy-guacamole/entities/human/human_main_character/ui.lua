local Preview = require 'data/scripts/preview'

local cg = require(Mod.getFilePath 'scripts/cg')

local buttons = {
    {'A', Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM},
    {'B', Widget.PositionPolicy.RIGHT    + Widget.PositionPolicy.CENTER_Y},
    {'X', Widget.PositionPolicy.LEFT     + Widget.PositionPolicy.CENTER_Y},
    {'Y', Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP}
}

local entityBackplateEnabledPicturePath  = 'mods/crispy-guacamole/ui/controller/entitybackplate_enabled.png'
local entityBackplateDisabledPicturePath = 'mods/crispy-guacamole/ui/controller/entitybackplate_disabled.png'

local function makeButton(parent, path)
    local container = Widget.makeFixedSize(70, 120)
    container:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
    container:setPadding(0)
    container:setMargin(0)

    local backplate
    do
        backplate = Widget.makeImage(entityBackplateDisabledPicturePath)
        backplate:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.CENTER_Y)
        
        local imageWidth, imageHeight = Image.getSize(entityBackplateDisabledPicturePath)
        backplate:setSize(
            imageWidth * 3,
            imageHeight * 3
        )
        container:addChild(backplate)
    end

    local previewContainer
    do
        previewContainer = Widget.makeExpand()
        container:addChild(previewContainer)
    end

    local gamepadButton
    do
        gamepadButton = Widget.makeImage(path)
        gamepadButton:setPositionPolicy(Widget.PositionPolicy.BOTTOM + Widget.PositionPolicy.CENTER_X)
        
        local imageWidth, imageHeight = Image.getSize(path)
        gamepadButton:setSize(
            imageWidth * 3,
            imageHeight * 3
        )
        container:addChild(gamepadButton)
    end

    parent:addChild(container)

    return {
        container        = container,
        backplate        = backplate,
        previewContainer = previewContainer,
        gamepadButton    = gamepadButton
    }
end

local function setButtonEnabled(buttonWidget, buttonName)
    buttonWidget.gamepadButton:setBackground('mods/crispy-guacamole/ui/controller/buttons/' .. buttonName .. '.png')
end

local function setBackplateEnabled(buttonWidget)
    buttonWidget.backplate:setBackground(entityBackplateEnabledPicturePath)
end

local function addEntityPreview(buttonWidget, entity)
    local entityPreview, offsetX, offsetY = Preview.entity(entity:getTemplatePath(), nil, true, 3)
    print(entity:getTemplateName(), entityPreview, offsetX, offsetY)
    entityPreview:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.CENTER_Y)
    entityPreview:setPosition(offsetX, offsetY - 12)
    buttonWidget.previewContainer:addChild(entityPreview)
end

local function entityJoinedGroup(playerEntity, otherEntity, buttonWidgets)
    local buttonNameForEntity = flat.tableFindValueKey(cg.entityNamePerButton, otherEntity:getTemplateName())
    local buttonWidget = buttonWidgets[buttonNameForEntity]
    setButtonEnabled(buttonWidget, buttonNameForEntity)
    setBackplateEnabled(buttonWidget)
    addEntityPreview(buttonWidget, otherEntity)
end

local function buildWidgets(playerEntity)
    local container = Widget.makeFixedSize(200, 240)
    container:setPositionPolicy(Widget.PositionPolicy.TOP + Widget.PositionPolicy.LEFT)
    container:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
    container:setPadding(5)
    container:setMargin(20)

    local buttonWidgets = {}
    for i = 1, #buttons do
        local buttonPicturePath = 'mods/crispy-guacamole/ui/controller/buttons/' .. buttons[i][1] .. '_disabled.png'
        local buttonPositionPolicy = buttons[i][2]
        local buttonWidget = makeButton(container, buttonPicturePath)
        buttonWidget.container:setPositionPolicy(buttonPositionPolicy)
        buttonWidgets[buttons[i][1]] = buttonWidget
    end

    Widget.getRoot():addChild(container)

    if playerEntity:getExtraData().entityJoinedGroup then
        playerEntity:getExtraData().entityJoinedGroup:on(function(entity)
            entityJoinedGroup(playerEntity, entity, buttonWidgets)
        end)
    end
end

local ui = {}

function ui.addedToMap(entity, widget)
    buildWidgets(entity)
end

return ui