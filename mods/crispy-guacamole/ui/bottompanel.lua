local Preview = require 'data/scripts/preview'
local Theme = require 'mods/crispy-guacamole/ui/theme'
local EntityData = require 'mods/crispy-guacamole/scripts/entitydata'

local root = Widget.getRoot()

local bottomPanel
local buildingContainer
local unitsContainer

local function makeBottomPanelContainer(title)
    local container = Widget.makeColumnFlow()
    container:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
    container:setSize(100, 0)
    container:setPadding(5)
    container:setMargin(0, 2, 4, 2)
    container:setBackgroundColor(Theme.BACKGROUND_COLOR)

    do
        local label = Widget.makeText(title, table.unpack(Theme.defaultFont))
        label:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP)
        label:setTextColor(Theme.TEXT_COLOR)
        container:addChild(label)
    end

    local content
    do
        content = Widget.makeColumnFlow()
        content:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
        container:addChild(content)
    end
    local function addContent(widget)
        content:addChild(widget)
    end
    local function clearContent()
        content:removeAllChildren()
    end

    bottomPanel:addChild(container)

    return {
        container    = container,
        addContent   = addContent,
        clearContent = clearContent
    }
end

local function buildWidgets()
    bottomPanel = Widget.makeLineFlow()
    bottomPanel:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM)
    bottomPanel:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.FIXED_Y)
    bottomPanel:setSize(0, 100)

    -- buildings
    do
        buildingContainer = makeBottomPanelContainer 'Building'
    end

    -- money
    do
        unitsContainer = makeBottomPanelContainer 'Units'
    end

    root:addChild(bottomPanel)
end

local function setBuilding(building)
    local buildingData = EntityData.get(building:getTemplateName())

    do
        do
            local label = Widget.makeText(buildingData.name, table.unpack(Theme.defaultFont))
            label:setPositionPolicy(Widget.PositionPolicy.CENTER)
            label:setTextColor(Theme.TEXT_COLOR)
            buildingContainer.addContent(label)
        end

        do
            local buildingPreview = Preview.entity(building:getTemplateName(), nil, true)
            buildingPreview:setPositionPolicy(Widget.PositionPolicy.CENTER)
            buildingContainer.addContent(buildingPreview)
        end
    end

    do
        local function spawnEntities(buildingPosition, numEntities, entityTemplateName)
            for i = 1, numEntities do
                local position = flat.Vector2(buildingPosition:x(), buildingPosition:y())
                position:x(position:x() + 1 + (math.random() - 0.5) * 0.01)
                position:y(position:y() + 1 + (math.random() - 0.5) * 0.01)
                Entity.spawn(entityTemplateName, position)
            end
        end

        for i = 1, #buildingData.units do
            local unit = buildingData.units[i]
            local unitData = EntityData.get(unit)

            local lineFlow = Widget.makeLineFlow()

            do
                local label = Widget.makeText(unitData.name, table.unpack(Theme.defaultFont))
                label:setTextColor(Theme.TEXT_COLOR)
                local position = building:getPosition():toVector2()
                label:click(function()
                    spawnEntities(position, 1, unit)
                end)
                lineFlow:addChild(label)
            end

            do
                local label = Widget.makeText(' x10', table.unpack(Theme.defaultFont))
                label:setTextColor(Theme.TEXT_COLOR)
                local position = building:getPosition():toVector2()
                label:click(function()
                    spawnEntities(position, 10, unit)
                end)
                lineFlow:addChild(label)
            end

            unitsContainer.addContent(lineFlow)
        end
    end
end

local function clearBuilding()
    buildingContainer.clearContent()
    unitsContainer.clearContent()
end

return {
    buildWidgets  = buildWidgets,
    setBuilding   = setBuilding,
    clearBuilding = clearBuilding
}