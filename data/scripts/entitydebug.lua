local Checkbox = require 'data/scripts/ui/checkbox'
local format = string.format

return function(addContainer, makeSeparator, font)

	local entityDebugContainer = addContainer 'Entity'

    local checkboxes = {}

    -- entity debug
    do
        local checkbox = Checkbox:new(true, 'Enable debug')
        checkbox:onValueChanged(function(_, value)
            for _, selectedEntity in Map.eachSelectedEntity() do
                selectedEntity:setDebug(value)
                if not value then
                    for componentName, componentFlag in pairs(Component) do
                        if selectedEntity:hasComponent(componentFlag) then
                            selectedEntity:setComponentDebug(componentFlag, value)
                        end
                    end
                end
            end
        end)

        entityDebugContainer:addChild(checkbox.container)
        local timer = flat.Timer()
        timer:onUpdate(function()
            local numEntities = 0
            local debugEnabled = false
            for _, selectedEntity in Map.eachSelectedEntity() do
                if selectedEntity:getDebug() then
                    debugEnabled = true
                end
                numEntities = numEntities + 1
            end

            local state = numEntities > 0 and 'default' or 'disabled'
            checkbox:setState(state)
            checkbox:setValue(debugEnabled)
        end)
        timer:start(0, true)
    end

    entityDebugContainer:addChild(makeSeparator())

    -- component debug
    do
        local label = Widget.makeText('Components', table.unpack(font))
        label:setTextColor(0x000000FF)
        entityDebugContainer:addChild(label)

        local componentsSorted = {}
        for componentName, componentFlag in pairs(Component) do
            componentsSorted[#componentsSorted + 1] = componentName
        end
        table.sort(componentsSorted)

        for i, componentName in pairs(componentsSorted) do
            local checkbox = Checkbox:new(true, componentName)

            local componentFlag = Component[componentName]
            checkbox:onValueChanged(function(_, value)
                for _, selectedEntity in Map.eachSelectedEntity() do
                    if selectedEntity:hasComponent(componentFlag) then
                        selectedEntity:setComponentDebug(componentFlag, value)
                        if value then
                            selectedEntity:setDebug(true)
                        end
                    end
                end
            end)

            entityDebugContainer:addChild(checkbox.container)
            checkboxes[componentName] = checkbox
        end
        local timer = flat.Timer()
        timer:onUpdate(function()
            local componentsToShow = {}
            for componentName, componentFlag in pairs(Component) do
                local hasComponent = false
                for _, selectedEntity in Map.eachSelectedEntity() do
                    if selectedEntity:hasComponent(componentFlag) then
                        hasComponent = true
                        break
                    end
                end

                local debugEnabled = false
                if hasComponent then
                    for _, selectedEntity in Map.eachSelectedEntity() do
                        if selectedEntity:hasComponent(componentFlag)
                            and selectedEntity:getComponentDebug(componentFlag) then
                            debugEnabled = true
                            break
                        end
                    end
                end

                local checkbox = checkboxes[componentName]
                local state = hasComponent and 'default' or 'disabled'
                checkbox:setState(state)
                checkbox:setValue(debugEnabled)
            end
        end)
        timer:start(0, true)
    end

    entityDebugContainer:addChild(makeSeparator())
    -- print extraData
    do
        local label = Widget.makeText('Print extra data', table.unpack(font))
        label:setTextColor(0x000000FF)
        entityDebugContainer:addChild(label)

        label:click(function()
            for _, selectedEntity in Map.eachSelectedEntity() do
                print(selectedEntity)
                flat.dump(selectedEntity:getExtraData())
            end
        end)
    end
    -- open in editor
    do
        local label = Widget.makeText('Open in editor', table.unpack(font))
        label:setTextColor(0x000000FF)
        entityDebugContainer:addChild(label)

        local entityTemplatePathsContainer = Widget.makeColumnFlow()
        entityDebugContainer:addChild(entityTemplatePathsContainer)

        local entityTemplateLabels = {}
        local timer = flat.Timer()
        timer:onUpdate(function()
            local selectedEntityTemplates = {}
            for _, selectedEntity in Map.eachSelectedEntity() do
                selectedEntityTemplates[selectedEntity:getTemplatePath()] = true
            end

            for entityTemplatePath in pairs(selectedEntityTemplates) do
                if not entityTemplateLabels[entityTemplatePath] then
                    local entityTemplateLabel = Widget.makeText(entityTemplatePath, table.unpack(font))
                    entityTemplateLabel:setTextColor(0x000000FF)
                    entityTemplateLabel:click(function()
                        EntityEditor.openEntity(Mod.getPath(), 'mods/crispy-guacamole/maps/sandbox', entityTemplatePath)
                        return true
                    end)
                    entityTemplatePathsContainer:addChild(entityTemplateLabel)
                    entityTemplateLabels[entityTemplatePath] = entityTemplateLabel
                end
            end

            for entityTemplatePath, entityTemplateLabel in pairs(entityTemplateLabels) do
                if not selectedEntityTemplates[entityTemplatePath] then
                    entityTemplateLabels[entityTemplatePath]:removeFromParent()
                    entityTemplateLabels[entityTemplatePath] = nil
                end
            end
        end)
        timer:start(0, true)
    end
end