local Preview = require 'data/scripts/preview'
local Theme = require 'mods/crispy-guacamole/ui/theme'
local EntityData = require 'mods/crispy-guacamole/scripts/entitydata'

local pairs = pairs

local root = Widget.getRoot()

local bottomPanel
local selectedEntityContainer
local buildEntitiesContainer

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

local function setBuilding(buildingTemplatePath, buildings)
    local buildingData = EntityData.get(buildingTemplatePath)

    do
        do
            local label = Widget.makeText(buildingData and buildingData.name or buildingTemplatePath, table.unpack(Theme.defaultFont))
            label:setPositionPolicy(Widget.PositionPolicy.CENTER)
            label:setTextColor(Theme.TEXT_COLOR)
            selectedEntityContainer.addContent(label)
        end

        do
            local unitPreview = Preview.entity(buildingTemplatePath, nil, true)
            unitPreview:setPositionPolicy(Widget.PositionPolicy.CENTER)
            selectedEntityContainer.addContent(unitPreview)
        end
    end

    do
        local function buildEntity(building, entityToBuild)
            local buildTimer = game.Timer()

            buildTimer:onUpdate(function(timer, elapsedTime)
                local t = elapsedTime / entityToBuild.duration
                if 0 < t and t < 1 then
                    building:getExtraData().progressChanged(t)
                end
            end)

            buildTimer:onEnd(function()
                local extraData = building:getExtraData()
                local buildingPosition = building:getPosition():toVector2()
                local position = flat.Vector2(buildingPosition:x(), buildingPosition:y())
                position:x(position:x() + 1 + (math.random() - 0.5) * 0.01)
                position:y(position:y() + 1 + (math.random() - 0.5) * 0.01)

                Entity.spawn(entityToBuild.entity, position)
                extraData.progressChanged(0)
                table.remove(extraData.queue,1)
                if #extraData.queue > 0 then 
                    buildEntity(building, extraData.queue[1])
                end
            end)
            buildTimer:start(entityToBuild.duration, false)
        end

        local function addToQueue(building, entityToBuild)
            local extraData = building:getExtraData()
            if extraData.queue == nil then
                extraData.queue = {}
            end
            local queue = extraData.queue
            queue[#queue + 1] = entityToBuild

            if #extraData.queue == 1 then 
                buildEntity(building, entityToBuild)
            end
        end
        
        if buildingData and buildingData.units then
            for i = 1, #buildingData.units do
                local unit = buildingData.units[i]
                local entityAsset = assert(Asset.findFromName('entity', unit.entity), 'Could not find entity asset ' .. unit.entity)
                local unitData = assert(EntityData.get(entityAsset:getPath()), 'Could not find unit data for ' .. entityAsset:getPath())
                local lineFlow = Widget.makeLineFlow()

                local position = buildings[1]:getPosition():toVector2()
                do
                    local label = Widget.makeText(unitData.name, table.unpack(Theme.defaultFont))
                    label:setTextColor(Theme.TEXT_COLOR)
                    label:click(function()
                        for i = 1, #buildings do
                            addToQueue(buildings[i], unit)
                        end
                    end)
                    lineFlow:addChild(label)
                end

                buildEntitiesContainer.addContent(lineFlow)
            end
        end
    end
end

local function setUnit(unitTemplatePath, units)
    local unitData = EntityData.get(unitTemplatePath)

    do
        do
            local label = Widget.makeText(unitData and unitData.name or unitTemplatePath, table.unpack(Theme.defaultFont))
            label:setPositionPolicy(Widget.PositionPolicy.CENTER)
            label:setTextColor(Theme.TEXT_COLOR)
            selectedEntityContainer.addContent(label)
        end

        do
            local unitPreview = Preview.entity(unitTemplatePath, nil, true, 3)
            unitPreview:setPositionPolicy(Widget.PositionPolicy.CENTER)
            selectedEntityContainer.addContent(unitPreview)
        end
    end

    do
        local function build(buildingAsset)
            local entityTemplatePath = buildingAsset:getPath()
            local buildingTypeData = EntityData.get(entityTemplatePath)
            local firstPos
            local path
            if buildingTypeData and buildingTypeData.pathType then
                game.setGhostEntity(
                    entityTemplatePath,
                    function(cursorPosition, tiles)
                        -- TODO: check same altitude and ground navigation capability
                        return { cursorPosition }
                    end,
                    function(tiles)
                        local bottomTilePosition = flat.Vector2(-math.huge, -math.huge)
                        tiles:eachTile(function(tile)
                            local x, y = Map.getTilePosition(tile)
                            if x + y > bottomTilePosition:x() + bottomTilePosition:y() then
                                bottomTilePosition:x(x)
                                bottomTilePosition:y(y)
                            end
                        end)
                        firstPos = bottomTilePosition
                        game.setGhostEntity(
                            entityTemplatePath,
                            function(cursorPosition, tiles)
                                local bottomTilePosition = flat.Vector2(-math.huge, -math.huge)
                                tiles:eachTile(function(tile)
                                    local x, y = Map.getTilePosition(tile)
                                    if x + y > bottomTilePosition:x() + bottomTilePosition:y() then
                                        bottomTilePosition:x(x)
                                        bottomTilePosition:y(y)
                                    end
                                end)
                                local newPath = Map.findPath(firstPos, bottomTilePosition, 1, Map.Navigability.GROUND)
                                if path == nil then
                                    return newPath:getUniqueTilePositions()
                                else
                                    local tmp = path:copy()
                                    tmp:insert(newPath)
                                    return tmp:getUniqueTilePositions()
                                end
                            end,
                            function(tiles, continueAction)
                                local bottomTilePosition = flat.Vector2(-math.huge, -math.huge)
                                tiles:eachTile(function(tile)
                                    local x, y = Map.getTilePosition(tile)
                                    if x + y > bottomTilePosition:x() + bottomTilePosition:y() then
                                        bottomTilePosition:x(x)
                                        bottomTilePosition:y(y)
                                    end
                                end)
                                if path ~= nil then
                                    path:insert(Map.findPath(firstPos, bottomTilePosition, 1, Map.Navigability.GROUND))
                                else
                                    path = Map.findPath(firstPos, bottomTilePosition, 1, Map.Navigability.GROUND)
                                end
                                firstPos = bottomTilePosition
                                if not continueAction then
                                    path:eachTile(function(position)
                                        -- spawn construction site and interact
                                        local building = Entity.spawn(entityTemplatePath, position)
                                        building:enterState 'under_construction'
                                        for _, unit in pairs(units) do
                                            if unit:canInteractWith(building) then
                                                unit:cancelCurrentActions()
                                                unit:interactWith(building)
                                            end
                                        end
                                    end)
                                end
                                return false, false
                            end)
                        return false, true
                    end
                )
            else
                game.setGhostEntity(
                    entityTemplatePath,
                    function(cursorPosition, tiles)
                        return { cursorPosition }
                    end,
                    function(tiles)
                        local bottomTilePosition = flat.Vector2(-math.huge, -math.huge)
                        tiles:eachTile(function(tile)
                            local x, y = Map.getTilePosition(tile)
                            if x + y > bottomTilePosition:x() + bottomTilePosition:y() then
                                bottomTilePosition:x(x)
                                bottomTilePosition:y(y)
                            end
                        end)
                        -- spawn construction site and interact
                        local building = Entity.spawn(entityTemplatePath, bottomTilePosition)
                        building:enterState 'under_construction'
                        for _, unit in pairs(units) do
                            if unit:canInteractWith(building) then
                                unit:cancelCurrentActions()
                                unit:interactWith(building)
                            end
                        end
                        return false, false
                    end
                )
            end
        end

        if unitData and unitData.buildings then
            for i = 1, #unitData.buildings do
                local buildingName = unitData.buildings[i]
                local buildingAsset = assert(Asset.findFromName('entity', buildingName), 'Could not find building ' .. buildingName)
                if buildingAsset then
                    local buildingData = assert(EntityData.get(buildingAsset:getPath()), 'Could not find building data for ' .. buildingAsset:getPath())

                    if buildingData then
                        local label = Widget.makeText(buildingData.name, table.unpack(Theme.defaultFont))
                        label:setTextColor(Theme.TEXT_COLOR)
                        label:click(function()
                            build(buildingAsset)
                        end)
                        buildEntitiesContainer.addContent(label)
                    end
                end
            end
        end
    end
end

local function buildWidgets()
    bottomPanel = Widget.makeLineFlow()
    bottomPanel:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM)
    bottomPanel:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.FIXED_Y)
    bottomPanel:setSize(0, 200)

    selectedEntityContainer = makeBottomPanelContainer 'Selection'
    buildEntitiesContainer = makeBottomPanelContainer 'Build'

    Map.selectionChanged(function()
        selectedEntityContainer.clearContent()
        buildEntitiesContainer.clearContent()

        local buildingsPerType = {}
        local unitsPerType = {}
        local entityTemplates = {}
        for _, selectedEntity in Map.eachSelectedEntity() do
            local entityTemplate = selectedEntity:getTemplatePath()
            local entityGroup
            if selectedEntity:hasComponent(Component.prop) then
                entityGroup = buildingsPerType
            else
                entityGroup = unitsPerType
            end
            if entityGroup[entityTemplate] then
                entityGroup[entityTemplate][#entityGroup[entityTemplate] + 1] = selectedEntity
            else
                entityGroup[entityTemplate] = { selectedEntity }
            end
        end

        for buildTemplatePath, buildings in pairs(buildingsPerType) do
            setBuilding(buildTemplatePath, buildings)
        end
        for unitTemplatePath, units in pairs(unitsPerType) do
            setUnit(unitTemplatePath, units)
        end
    end)

    root:addChild(bottomPanel)
end

return {
    buildWidgets = buildWidgets
}