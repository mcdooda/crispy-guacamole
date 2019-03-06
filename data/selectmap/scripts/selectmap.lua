local UiSettings = require 'data/scripts/ui/uisettings'

local modPath = Mod.getPath()
assert(modPath and #modPath > 0)
local maps = Mod.getMaps()
local entities = Mod.getEntities()

local function getModDirectories()
    local mods = {}
    local modDirectoryPath = flat.Directory(modPath):getParentPath()
    local modDirectory = flat.Directory(modDirectoryPath)
    modDirectory:eachSubFile(function(file)
        if file:isDirectory() then
            mods[#mods + 1] = file:getPath()
        end
    end)
    return mods
end

local boxContainer = Widget.makeLineFlow()
boxContainer:setPositionPolicy(Widget.PositionPolicy.CENTER)
boxContainer:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.EXPAND_Y)
boxContainer:setMargin(20)

-- mod path
do
    local modPathLabel = Widget.makeText(modPath, table.unpack(UiSettings.defaultFont))
    modPathLabel:setMargin(5)
    modPathLabel:click(flat.ui.task(function()
        local mods = getModDirectories()
        local modToOpen = flat.ui.choice('Open mod:', mods, modPath)
        if modToOpen ~= modPath then
            Mod.openEditor(modToOpen)
        end
    end))
    Widget.getRoot():addChild(modPathLabel)
end

-- maps
do
    local box = Widget.makeColumnFlow()
    box:setBackgroundColor(0x666666FF)
    box:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
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
            newMapLabel:click(flat.ui.task(function()
                local mapName = flat.ui.prompt 'Map name:'
                if mapName and mapName ~= '' then
                    mapName = mapName:lower()
                    loadfile('data/editor/tools/newmap.lua')(mapName)
                    MapEditor.openMap(modPath, mapName)
                end
            end))
            mapsTitleLine:addChild(newMapLabel)
        end

        box:addChild(mapsTitleLine)
    end

    do
        local mapListBox = Widget.makeColumnFlow()
        mapListBox:setSizePolicy(Widget.SizePolicy.EXPAND)
        mapListBox:setAllowScrollY(true)

        for i = 1, #maps do
            local mapName = maps[i]
            local mapLine = Widget.makeLineFlow()
            mapLine:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

            local mapLabel = Widget.makeText(mapName, table.unpack(UiSettings.titleFont))
            mapLabel:setMargin(0, 5, 0, 5)
            mapLabel:click(function()
                game.openMap(modPath, mapName)
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
                MapEditor.openMap(modPath, mapName)
                return true
            end)
            mapLine:addChild(editLabel)

            mapListBox:addChild(mapLine)
        end

        box:addChild(mapListBox)
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
    box:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
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
            newEntityLabel:click(flat.ui.task(function()
                local entityName = flat.ui.prompt 'Entity name:'
                if entityName and entityName ~= '' then
                    entityName = entityName:lower()
                    loadfile('data/editor/tools/newentity.lua')(entityName)
                    EntityEditor.openEntity(modPath, 'sandbox', entityName)
                end
                return true
            end))
            entitiesTitleLine:addChild(newEntityLabel)
        end

        box:addChild(entitiesTitleLine)
    end

    do
        local entityListBox = Widget.makeColumnFlow()
        entityListBox:setSizePolicy(Widget.SizePolicy.EXPAND)
        entityListBox:setAllowScrollY(true)

        for i = 1, #entities do
            local entityName = entities[i]
            local entityLine = Widget.makeLineFlow()
            entityLine:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

            local entityLabel = Widget.makeText(entityName, table.unpack(UiSettings.titleFont))
            entityLabel:setMargin(0, 5, 0, 5)
            entityLabel:click(function()
                EntityEditor.openEntity(modPath, 'sandbox', entityName)
                return true
            end)
            entityLine:addChild(entityLabel)

            entityListBox:addChild(entityLine)
        end

        box:addChild(entityListBox)
    end

    boxContainer:addChild(box)
end

Widget.getRoot():addChild(boxContainer)
