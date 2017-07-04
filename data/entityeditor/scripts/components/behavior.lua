local UiSettings = require 'data/scripts/ui/uisettings'
local Icon = require 'data/scripts/ui/icon'

return function(componentDetailsPanel, entityTemplateName, componentTemplate, entity)
    local states = {}
    local inputs = {}

    for stateName, state in pairs(componentTemplate) do
        local isInput = stateName:sub(1, 2) == 'on'
        if isInput then
            inputs[#inputs + 1] = stateName
        else
            states[#states + 1] = stateName
        end
    end
    table.sort(states)
    table.sort(inputs)

    do
        do
            local label = Widget.makeText(
                'States',
                table.unpack(UiSettings.defaultFont)
            )
            label:setMargin(2)
            componentDetailsPanel:addChild(label)
        end

        for i = 1, #states do
            local stateName = states[i]

            local stateLine = Widget.makeLineFlow()

            do
                local stateLabel = Widget.makeText(
                    stateName,
                    table.unpack(UiSettings.defaultFont)
                )
                stateLabel:setMargin(0, 0, 0, 8)
                stateLine:addChild(stateLabel)
            end

            do
                local playIcon = Icon:new('play', 10)
                playIcon.container:setMargin(0, 0, 1, 3)
                playIcon.container:click(function()
                    if not entity:isComponentEnabled(Component.behavior) then
                        entity:decComponentDisableLevel(Component.behavior)
                    end
                    local enteredState, errorMessage = pcall(function()
                        entity:enterState(stateName, false)
                    end)
                    if not enteredState then
                        pcall(function()
                            entity:enterState('init', false)
                        end)
                        entity:incComponentDisableLevel(Component.behavior)
                    end
                end)
                stateLine:addChild(playIcon.container)
            end

            componentDetailsPanel:addChild(stateLine)
        end
    end

    do
        do
            local label = Widget.makeText(
                'Inputs',
                table.unpack(UiSettings.defaultFont)
            )
            label:setMargin(7, 2, 2, 2)
            componentDetailsPanel:addChild(label)
        end

        for i = 1, #inputs do
            local inputLabel = Widget.makeText(
                inputs[i],
                table.unpack(UiSettings.defaultFont)
            )
            inputLabel:setMargin(0, 0, 0, 8)
            componentDetailsPanel:addChild(inputLabel)
        end
    end
end