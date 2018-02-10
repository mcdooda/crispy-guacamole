local UiSettings = require 'data/scripts/ui/uisettings'
local Preview = require 'data/scripts/preview'
local Icon = require 'data/scripts/ui/icon'

return function(componentDetailsPanel, entityTemplateName, componentTemplate, getEntity)
    do
        local label = Widget.makeText(
            'Speed: ' .. componentTemplate.speed,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local label = Widget.makeText(
            'Weight: ' .. componentTemplate.weight,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local shootLine = Widget.makeLineFlow()
        shootLine:setMargin(2)

        do
            local shootLabel = Widget.makeText(
                'Shoot',
                table.unpack(UiSettings.defaultFont)
            )
            shootLine:addChild(shootLabel)
        end

        do
            local playIcon = Icon:new('play', 10)
            playIcon.container:click(function()
                local entity = getEntity()
                if not entity:isComponentEnabled(Component.projectile) then
                    entity:decComponentDisableLevel(Component.projectile)
                end
            end)
            shootLine:addChild(playIcon.container)
        end

        componentDetailsPanel:addChild(shootLine)
    end
end