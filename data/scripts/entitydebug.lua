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

        Timer.start(0, function()
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
        end, nil,  true)
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

        Timer.start(0, function()
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
        end, nil, true)
    end

    entityDebugContainer:addChild(makeSeparator())

    -- open in editor
    do
        local label = Widget.makeText('Open in editor', table.unpack(font))
        label:setTextColor(0x000000FF)
        entityDebugContainer:addChild(label)

        local entityTemplateNamesContainer = Widget.makeColumnFlow()
        entityDebugContainer:addChild(entityTemplateNamesContainer)

        local entityTemplateLabels = {}

        Timer.start(0, function()
            local selectedEntityTemplates = {}
            for _, selectedEntity in Map.eachSelectedEntity() do
                selectedEntityTemplates[selectedEntity:getTemplateName()] = true
            end

            for entityTemplateName in pairs(selectedEntityTemplates) do
                if not entityTemplateLabels[entityTemplateName] then
                    local entityTemplateLabel = Widget.makeText(entityTemplateName, table.unpack(font))
                    entityTemplateLabel:setTextColor(0x000000FF)
                    entityTemplateLabel:click(function()
                        EntityEditor.openEntity(Mod.getPath(), 'sandbox', entityTemplateName)
                        return true
                    end)
                    entityTemplateNamesContainer:addChild(entityTemplateLabel)
                    entityTemplateLabels[entityTemplateName] = entityTemplateLabel
                end
            end

            for entityTemplateName, entityTemplateLabel in pairs(entityTemplateLabels) do
                if not selectedEntityTemplates[entityTemplateName] then
                    entityTemplateLabels[entityTemplateName]:removeFromParent()
                    entityTemplateLabels[entityTemplateName] = nil
                end
            end
        end, nil, true)
    end
end