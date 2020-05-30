local UiSettings = require 'data/scripts/ui/uisettings'
local Icon = require 'data/scripts/ui/icon'

return function(componentDetailsPanel, entityTemplatePath, componentTemplate, getEntity)
    local states = {}
    local events = {}

    for stateName, state in pairs(componentTemplate) do
        local isEvent = stateName:sub(1, 2) == 'on'
        if isEvent then
            events[#events + 1] = stateName
        else
            states[#states + 1] = stateName
        end
    end
    table.sort(states)
    table.sort(events)

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
                playIcon.container:setMargin(0, 0, 0, 3)
                playIcon.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
                playIcon.container:click(function()
                    local entity = getEntity()
                    if not entity:isComponentEnabled(Component.behavior) then
                        entity:decComponentDisableLevel(Component.behavior)
                    end
                    local enteredState, errorMessage = pcall(function()
                        entity:enterState(stateName)
                    end)
                    if not enteredState then
                        flat.ui.error(errorMessage)
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
                'Events',
                table.unpack(UiSettings.defaultFont)
            )
            label:setMargin(7, 2, 2, 2)
            componentDetailsPanel:addChild(label)
        end

        for i = 1, #events do
            local eventLabel = Widget.makeText(
                events[i],
                table.unpack(UiSettings.defaultFont)
            )
            eventLabel:setMargin(0, 0, 0, 8)
            componentDetailsPanel:addChild(eventLabel)
        end
    end
end