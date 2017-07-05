local UiSettings = require 'data/scripts/ui/uisettings'
local Preview = require 'data/scripts/preview'
local Icon = require 'data/scripts/ui/icon'

return function(componentDetailsPanel, entityTemplateName, componentTemplate, getEntity)
    do
        local label = Widget.makeText(
            'Range: ' .. componentTemplate.attackRange,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local label = Widget.makeText(
            'Cooldown: ' .. componentTemplate.attackCooldown,
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local label = Widget.makeText(
            'Auto attack: ' .. (componentTemplate.autoAttack and 'true' or 'false'),
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
    do
        local label = Widget.makeText(
            'Move during attack: ' .. (componentTemplate.moveDuringAttack and 'true' or 'false'),
            table.unpack(UiSettings.defaultFont)
        )
        label:setMargin(2)
        componentDetailsPanel:addChild(label)
    end
end