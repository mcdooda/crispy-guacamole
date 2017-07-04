local UiSettings = require 'data/scripts/ui/uisettings'
local Path = require 'data/scripts/path'

local root = Widget.getRoot()

local selectedBackgroundColor = 0x666666FF
local componentDisabledColor = 0x999999FF
local componentEnabledColor = 0xFFFFFFFF

do
    local leftPanel = Widget.makeLineFlow()
    leftPanel:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.EXPAND_Y)

    local componentsSorted = {}
    for componentName, componentFlag in pairs(Component) do
        componentsSorted[#componentsSorted + 1] = componentName
    end
    table.sort(componentsSorted)

    local startZone = Map.getZone 'Start'
    local zoneCenter = startZone:getCenter()
    local entity = startZone:getEntities()[1]
    entity:setDebug(true)
    local entityTemplateName = entity:getTemplateName()

    local setComponentTab
    local currentComponentName = ''

    -- component names
    local componentTabs = {}
    do
        local componentsPanel = Widget.makeColumnFlow()
        componentsPanel:setBackgroundColor(0x444444FF)
        componentsPanel:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.EXPAND_Y)

        do
            local titleLabel = Widget.makeText('Components', table.unpack(UiSettings.titleFont))
            titleLabel:setMargin(2, 5, 2, 5)
            componentsPanel:addChild(titleLabel)
        end

        for i, componentName in pairs(componentsSorted) do
            local componentFlag = Component[componentName]
            local hasComponent = entity:hasComponent(componentFlag)

            local componentTab = Widget.makeColumnFlow()
            componentTab:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
            local componentNameLabel = Widget.makeText(componentName, table.unpack(UiSettings.defaultFont))
            if hasComponent then
                componentNameLabel:setTextColor(componentEnabledColor)
                if selectedComponentName ~= componentName then
                    componentTab:click(function()
                        if currentComponentName ~= componentName then
                            setComponentTab(currentComponentName, componentName)
                        end
                    end)
                end
            else
                componentNameLabel:setTextColor(componentDisabledColor)
            end

            componentTab:setPadding(1, 5, 1, 5)
            componentTab:addChild(componentNameLabel)

            componentsPanel:addChild(componentTab)
            componentTabs[componentName] = componentTab
        end

        leftPanel:addChild(componentsPanel)
    end

    -- component details
    local selectedComponentPanel
    do
        selectedComponentPanel = Widget.makeColumnFlow()
        selectedComponentPanel:setBackgroundColor(selectedBackgroundColor)
        selectedComponentPanel:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
        selectedComponentPanel:setSize(200, 0)
        leftPanel:addChild(selectedComponentPanel)
    end

    local function disableTab(componentName)
        componentTabs[componentName]:setBackgroundColor(0x00000000)

        entity:setComponentDebug(Component[componentName], false)
    end

    local function enableTab(componentName)
        componentTabs[componentName]:setBackgroundColor(selectedBackgroundColor)

        entity:setComponentDebug(Component[componentName], true)
    end

    function setComponentTab(previousSelectedComponentName, selectedComponentName)
        currentComponentName = selectedComponentName

        -- disable previous component debug
        if previousSelectedComponentName then
            disableTab(previousSelectedComponentName)
        end

        -- enable current component debug
        enableTab(selectedComponentName)

        -- update common information then load custom information from components/<componentName>
        do
            selectedComponentPanel:removeAllChildren()

            if selectedComponentName and #selectedComponentName > 0 then
                do
                    local titleLine = Widget.makeLineFlow()

                    local title = selectedComponentName:sub(1, 1):upper() .. selectedComponentName:sub(2)
                    local titleLabel = Widget.makeText(title, table.unpack(UiSettings.titleFont))
                    titleLabel:setMargin(2, 0, 0, 5)
                    titleLine:addChild(titleLabel)

                    local componentEnabled = entity:isComponentEnabled(Component[selectedComponentName])
                    local enabledLabel = Widget.makeText('(on)', table.unpack(UiSettings.defaultFont))
                    enabledLabel:setTextColor(0xCCCCCCFF)
                    enabledLabel:setMargin(0, 0, 5, 5)
                    if not componentEnabled then
                        enabledLabel:setText('(off)')
                    end
                    titleLine:addChild(enabledLabel)

                    selectedComponentPanel:addChild(titleLine)
                end

                local componentDetailsPanel = Widget.makeColumnFlow()
                componentDetailsPanel:setPadding(5)
                local hasDetails, showComponentDetails = pcall(function()
                    return require('data/entityeditor/scripts/components/' .. selectedComponentName)
                end)
                if hasDetails then
                    local componentTemplate = Path.requireComponentTemplate(entityTemplateName, selectedComponentName)
                    showComponentDetails(componentDetailsPanel, entityTemplateName, componentTemplate, entity)
                end
                selectedComponentPanel:addChild(componentDetailsPanel)
            end
        end
    end

    local function findFirstEnabledComponent()
        for i, componentName in pairs(componentsSorted) do
            local componentFlag = Component[componentName]
            if entity:hasComponent(componentFlag) then
                return componentName
            end
        end
    end

    setComponentTab(nil, findFirstEnabledComponent())

    root:addChild(leftPanel)
end