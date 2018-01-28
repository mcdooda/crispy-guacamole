local UiSettings = require 'data/scripts/ui/uisettings'

local defaultMod = 'mods/crispy-guacamole'
local maps = require(defaultMod .. '/maps/maps')
local entities = require(defaultMod .. '/entities/entities')

local boxContainer = Widget.makeLineFlow()
boxContainer:setPositionPolicy(Widget.PositionPolicy.CENTER)

-- maps
do
    local box = Widget.makeColumnFlow()
    box:setBackgroundColor(0x666666FF)
    box:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.COMPRESS_Y)
    box:setSize(300, 0)

    do
        local mapsTitleLine = Widget.makeLineFlow()
        mapsTitleLine:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

        do
            local mapsLabel = Widget.makeText('- Maps -', table.unpack(UiSettings.titleFont))
            mapsLabel:setMargin(5)
            mapsTitleLine:addChild(mapsLabel)
        end

        do
            local spacer = Widget.makeExpand()
            mapsTitleLine:addChild(spacer)
        end

        do
            local newMapLabel = Widget.makeText('New', table.unpack(UiSettings.titleFont))
            newMapLabel:setMargin(5)
            newMapLabel:click(function()
                MapEditor.newMap()
                return true
            end)
            mapsTitleLine:addChild(newMapLabel)
        end

        box:addChild(mapsTitleLine)
    end

    for i = 1, #maps do
        local mapName = maps[i]
        local mapLine = Widget.makeLineFlow()
        mapLine:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

        local mapLabel = Widget.makeText(mapName, table.unpack(UiSettings.titleFont))
        mapLabel:setMargin(0, 5, 0, 5)
        mapLabel:click(function()
            Game.openMap(defaultMod, mapName)
            return true
        end)
        mapLine:addChild(mapLabel)

        do
            local spacer = Widget.makeExpand()
            mapLine:addChild(spacer)
        end

        local editLabel = Widget.makeText('Edit', table.unpack(UiSettings.titleFont))
        editLabel:setMargin(0, 5, 0, 5)
        editLabel:click(function()
            MapEditor.openMap(defaultMod, mapName)
            return true
        end)
        mapLine:addChild(editLabel)

        box:addChild(mapLine)
    end

    boxContainer:addChild(box)
end

-- spacer
do
    local spacer = Widget.makeFixedSize(20, 1)
    boxContainer:addChild(spacer)
end

-- entities
do
    local box = Widget.makeColumnFlow()
    box:setBackgroundColor(0x666666FF)
    box:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.COMPRESS_Y)
    box:setSize(300, 0)

    do
        local entitiesTitleLine = Widget.makeLineFlow()
        entitiesTitleLine:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

        do
            local entitiesLabel = Widget.makeText('- Entities -', table.unpack(UiSettings.titleFont))
            entitiesLabel:setMargin(5)
            entitiesTitleLine:addChild(entitiesLabel)
        end

        do
            local spacer = Widget.makeExpand()
            entitiesTitleLine:addChild(spacer)
        end

        do
            local newEntityLabel = Widget.makeText('New', table.unpack(UiSettings.titleFont))
            newEntityLabel:setMargin(5)
            newEntityLabel:click(function()
                EntityEditor.newEntity()
                return true
            end)
            entitiesTitleLine:addChild(newEntityLabel)
        end

        box:addChild(entitiesTitleLine)
    end

    for i = 1, #entities do
        local entityName = entities[i]
        local entityLine = Widget.makeLineFlow()
        entityLine:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

        local entityLabel = Widget.makeText(entityName, table.unpack(UiSettings.titleFont))
        entityLabel:setMargin(0, 5, 0, 5)
        entityLabel:click(function()
            EntityEditor.openEntity(defaultMod, 'sandbox', entityName)
            return true
        end)
        entityLine:addChild(entityLabel)

        box:addChild(entityLine)
    end

    boxContainer:addChild(box)
end

Widget.getRoot():addChild(boxContainer)
