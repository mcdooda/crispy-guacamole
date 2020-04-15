local UiSettings = require 'data/scripts/ui/uisettings'

dofile 'data/tool/init.lua'

local modPath = Mod.getPath()
assert(modPath and #modPath > 0)

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
        if modToOpen and modToOpen ~= modPath then
            Mod.openEditor(modToOpen)
        end
    end))
    Widget.getRoot():addChild(modPathLabel)
end

-- compounds
do
    flat.graph.loadNodeClasses('script', 'data')
    local modCompoundsPath = Mod.getPath() .. '/compounds'
    local dataCompoundsPath = 'data/compounds'
    flat.graph.loadCompounds('script', modCompoundsPath)
    flat.graph.loadCompounds('script', dataCompoundsPath)
    local compounds = flat.graph.getCompounds 'script'

    local box = Widget.makeColumnFlow()
    box:setBackgroundColor(0x666666FF)
    box:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
    box:setSize(300, 0)

    do
        local compoundsTitleLine = Widget.makeLineFlow()
        compoundsTitleLine:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

        do
            local compoundsLabel = Widget.makeText('- Compounds -', table.unpack(UiSettings.titleFont))
            compoundsLabel:setMargin(5)
            compoundsTitleLine:addChild(compoundsLabel)
        end

        do
            local spacer = Widget.makeExpand()
            compoundsTitleLine:addChild(spacer)
        end

        do
            local newMapLabel = Widget.makeText('New', table.unpack(UiSettings.titleFont))
            newMapLabel:setMargin(5)
            newMapLabel:click(flat.ui.task(function()
                local compoundName = flat.ui.prompt 'Compound name:'
                if compoundName and compoundName ~= '' then
                    error 'TODO'
                end
            end))
            compoundsTitleLine:addChild(newMapLabel)
        end

        box:addChild(compoundsTitleLine)
    end

    do
        local compoundListBox = Widget.makeColumnFlow()
        compoundListBox:setSizePolicy(Widget.SizePolicy.EXPAND)
        compoundListBox:setAllowScrollY(true)

        local compoundList = {}
        for i = 1, #compounds do
            local origin
            local compoundPath = compounds[i].path
            if compoundPath:sub(1, #modCompoundsPath) == modCompoundsPath then
                origin = 'mod'
            elseif compoundPath:sub(1, #dataCompoundsPath) == dataCompoundsPath then
                origin = 'shared'
            end
            flat.arrayAdd(compoundList, {
                name = compounds[i].name,
                path = compoundPath,
                origin = origin
            })
        end
        table.sort(
            compoundList,
            function(a, b)
                if a.origin == b.origin then
                    return a.name < b.name
                end
                return a.origin < b.origin
            end
        )

        for i = 1, #compoundList do
            local compoundName = compoundList[i].name

            local compoundLine = Widget.makeLineFlow()
            compoundLine:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

            local compoundLabel = Widget.makeText(compoundName, table.unpack(UiSettings.titleFont))
            compoundLabel:setMargin(0, 0, 0, 5)
            compoundLabel:click(function()
                flat.graph.editor.open(
                    Widget.getRoot(),
                    compoundList[i].path,
                    'script'
                )
                return true
            end)
            compoundLine:addChild(compoundLabel)

            local originLabel = Widget.makeText('[' .. compoundList[i].origin .. ']', table.unpack(UiSettings.menuButtonFont))
            originLabel:setMargin(3, 0, 0, 3)
            compoundLine:addChild(originLabel)

            compoundListBox:addChild(compoundLine)
        end

        box:addChild(compoundListBox)
    end

    boxContainer:addChild(box)
end

Widget.getRoot():addChild(boxContainer)

flat.tool.asset.browser(Widget.getRoot(), modPath)
