local healthBarWidgetWidth = 22
local healthBarWidgetHeight = 6

local healthBarInnerWidgetMaxWidth = healthBarWidgetWidth - 2
local healthBarInnerWidgetHeight = healthBarWidgetHeight - 2

return {
    addedToMap = function(entity, widget, offsetY)
        local healthBarWidget = Widget.makeFixedSize(healthBarWidgetWidth, healthBarWidgetHeight)
        healthBarWidget:setBackground 'mods/dragons-lair/ui/entity/healthbarbg.png'

        local healthBarInnerWidget = Widget.makeFixedSize(healthBarWidgetWidth - 2, healthBarWidgetHeight - 2)
        healthBarInnerWidget:setBackground 'mods/dragons-lair/ui/entity/healthbarfg.png'
        healthBarInnerWidget:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
        healthBarInnerWidget:setPosition(1, -1)
        healthBarWidget:addChild(healthBarInnerWidget)

        widget:addChild(healthBarWidget)

        Widget.getRoot():addChild(widget)

        entity:setUiVisible(false)
        entity:setUiOffset(flat.Vector2(-healthBarWidgetWidth / 2, offsetY))
        entity:healthChanged(function(previousHealth, currentHealth, maxHealth)
            local healthRatio = currentHealth / maxHealth
            if 0 < healthRatio and healthRatio < 1 then
                local healthBarInnerWidgetWidth = healthRatio * healthBarInnerWidgetMaxWidth
                entity:setUiVisible(true)
                healthBarInnerWidget:setSize(healthBarInnerWidgetWidth, healthBarInnerWidgetHeight)
            else
                entity:setUiVisible(false)
            end
        end)
    end,

    removedFromMap = function(entity, widget)
        widget:removeFromParent()
    end
}