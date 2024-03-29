local UiSettings = require 'data/scripts/ui/uisettings'
local Icon = require 'data/scripts/ui/icon'

return function(componentDetailsPanel, entityTemplatePath, componentTemplate, getEntity)
    do
        local label = Widget.makeText(
            'Max health: ' .. componentTemplate.maxHealth,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local killLine = Widget.makeLineFlow()
        killLine:setMargin(2)

        do
            local killLabel = Widget.makeText(
                'Kill',
                table.unpack(UiSettings.defaultFont)
            )
            killLine:addChild(killLabel)
        end

        do
            local playIcon = Icon:new('play', 10)
            playIcon.container:setMargin(0, 0, 0, 3)
            playIcon.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
            playIcon.container:click(function()
                local entity = getEntity()
                local killed, errorMessage = pcall(function()
                    entity:kill()
                end)
                if not killed then
                    flat.ui.error(errorMessage)
                end
            end)
            killLine:addChild(playIcon.container)
        end

        componentDetailsPanel:addChild(killLine)
    end
end