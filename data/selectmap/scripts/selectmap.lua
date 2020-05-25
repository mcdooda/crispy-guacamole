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

local function addSpacer()
    local spacer = Widget.makeFixedSize(20, 1)
    boxContainer:addChild(spacer)
end

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

-- asset browser
do
    local box = Widget.makeColumnFlow()
    box:setBackgroundColor(0x666666FF)
    box:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
    box:setSize(300, 0)

    do
        local browserContainer = Widget.makeColumnFlow()
        browserContainer:setSizePolicy(Widget.SizePolicy.EXPAND)
        browserContainer:setAllowScrollY(true)
        box:addChild(browserContainer)

        flat.tool.asset.browser(browserContainer, modPath)
    end

    boxContainer:addChild(box)
end

addSpacer()

-- compounds
local function addCompoundColumn(nodeType, columnName, directories)
    flat.graph.loadNodeClasses(nodeType, 'data')
    for directory, _ in pairs(directories) do
        flat.graph.loadCompounds(nodeType, directory)
    end
    local compounds = flat.graph.getCompounds(nodeType)

    local box = Widget.makeColumnFlow()
    box:setBackgroundColor(0x666666FF)
    box:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
    box:setSize(300, 0)

    do
        local compoundsTitleLine = Widget.makeLineFlow()
        compoundsTitleLine:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

        do
            local compoundsLabel = Widget.makeText('- ' .. columnName .. ' -', table.unpack(UiSettings.titleFont))
            compoundsLabel:setMargin(5)
            compoundsTitleLine:addChild(compoundsLabel)
        end

        do
            local spacer = Widget.makeExpand()
            compoundsTitleLine:addChild(spacer)
        end

        do
            local newCompoundLabel = Widget.makeText('New', table.unpack(UiSettings.titleFont))
            newCompoundLabel:setMargin(5)
            newCompoundLabel:click(flat.ui.task(function()
                local compoundName = flat.ui.prompt 'Compound name:'
                if compoundName and compoundName ~= '' then
                    error 'TODO'
                end
            end))
            compoundsTitleLine:addChild(newCompoundLabel)
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
            for directory, originName in pairs(directories) do
                if compoundPath:sub(1, #directory) == directory then
                    origin = originName
                    break
                end
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
                    nodeType
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

addCompoundColumn('script', 'Compounds', {
    [Mod.getPath() .. '/compounds'] = 'mod',
    ['data/compounds']              = 'shared'
})

addSpacer()

addCompoundColumn('sound', 'Sounds', {
    [Mod.getPath() .. '/sounds'] = 'mod',
})


Widget.getRoot():addChild(boxContainer)

--flat.tool.asset.openBrowserWindow(Widget.getRoot(), modPath)
