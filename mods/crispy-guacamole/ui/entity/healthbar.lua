local healthBarWidgetWidth = 22
local healthBarWidgetHeight = 6

return {
    addedToMap = function(entity, widget)
        local healthBarExtraData = {}

        do
            local healthBarWidget = Widget.makeFixedSize(healthBarWidgetWidth, healthBarWidgetHeight)
            healthBarWidget:setBackground 'mods/crispy-guacamole/ui/entity/healthbarbg.png'

            do
                local healthBarInnerWidget = Widget.makeFixedSize(healthBarWidgetWidth - 2, healthBarWidgetHeight - 2)
                healthBarInnerWidget:setBackground 'mods/crispy-guacamole/ui/entity/healthbarfg.png'
                healthBarInnerWidget:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
                healthBarInnerWidget:setPosition(1, -1)

                healthBarWidget:addChild(healthBarInnerWidget)

                healthBarExtraData.healthBarInnerWidget = healthBarInnerWidget
            end

            widget:addChild(healthBarWidget)
        end

        entity:getExtraData().healthBarExtraData = healthBarExtraData

        local root = Widget.getRoot()
        root:addChild(widget)
    end,

    removedFromMap = function(entity, widget)
        widget:removeFromParent()
    end,

    update = function(entity, widget, offsetY)
        local x, y = entity:getUiPosition()
        offsetY = offsetY or 0
        widget:setPosition(x - healthBarWidgetWidth / 2, y + offsetY)

        local healthBarInnerWidget = entity:getExtraData().healthBarExtraData.healthBarInnerWidget
        local healthBarInnerWidgetWidth = entity:getHealth() / entity:getMaxHealth() * (healthBarWidgetWidth - 2)

        if healthBarInnerWidgetWidth > 0 then
            widget:setVisible(true)
            healthBarInnerWidget:setSize(healthBarInnerWidgetWidth, healthBarWidgetHeight - 2)
        else
            widget:setVisible(false)
        end
    end
}