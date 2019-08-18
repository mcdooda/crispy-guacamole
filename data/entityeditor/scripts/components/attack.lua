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
    do
        local attackLine = Widget.makeLineFlow()
        attackLine:setMargin(2)

        do
            local attackLabel = Widget.makeText(
                'Spawn attack target',
                table.unpack(UiSettings.defaultFont)
            )
            attackLine:addChild(attackLabel)
        end

        do
            local playIcon = Icon:new('play', 10)
            playIcon.container:setMargin(0, 0, 0, 5)
            playIcon.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
            playIcon.container:click(function()
                local entity = getEntity()
                
                local targetZone = Map.getZone 'Target'
                local zoneCenter = targetZone:getCenter()

                local target = Entity.spawn('zombie', zoneCenter)
                entity:setAttackTarget(target)
            end)
            attackLine:addChild(playIcon.container)
        end

        componentDetailsPanel:addChild(attackLine)
    end
end