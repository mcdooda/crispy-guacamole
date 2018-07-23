local Theme = require 'mods/crispy-guacamole/ui/theme'

local mineralsAmountWidgetWidth = 32

return {
    addedToMap = function(entity, widget)
        local amountLabel
        do
            local amountContainer = Widget.makeFixedSize(mineralsAmountWidgetWidth, 1)
            amountContainer:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.COMPRESS_Y)
            amountContainer:setBackgroundColor(Theme.MINERALS_BACKGROUND_COLOR)

            do
                amountLabel = Widget.makeText(entity:getExtraData().amount, table.unpack(Theme.mineralsFont))
                amountLabel:setTextColor(Theme.MINERALS_TEXT_COLOR)
                amountLabel:setPositionPolicy(Widget.PositionPolicy.CENTER)
                amountContainer:addChild(amountLabel)
            end

            widget:addChild(amountContainer)
        end

        entity:setUiOffset(flat.Vector2(-mineralsAmountWidgetWidth / 2, 20))

        Widget.getRoot():addChild(widget)

        entity:getExtraData().amountChanged:on(function(amount)
            amountLabel:setText(amount)
        end)
    end,

    removedFromMap = function(entity, widget)
        widget:removeFromParent()
    end
}