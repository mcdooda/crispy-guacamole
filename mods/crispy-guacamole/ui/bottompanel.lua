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
        for i = 1, #buildingData.units do
            local unit = buildingData.units[i]
            local unitData = EntityData.get(unit)
            local label = Widget.makeText(unitData.name, table.unpack(Theme.defaultFont))
            label:setTextColor(Theme.TEXT_COLOR)
            label:click(function()
                local position = building:getPosition():toVector2()
                position:x(position:x() + 1 + (math.random() - 0.5) * 0.01)
                position:y(position:y() + 1 + (math.random() - 0.5) * 0.01)
                local entity = Entity.spawn(unit, position)
            end)
            unitsContainer.addContent(label)
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