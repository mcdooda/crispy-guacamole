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
            local stateLabel = Widget.makeText(
                states[i],
                table.unpack(UiSettings.defaultFont)
            )
            stateLabel:setMargin(0, 0, 0, 8)
            componentDetailsPanel:addChild(stateLabel)
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