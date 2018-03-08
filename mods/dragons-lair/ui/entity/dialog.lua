local Path  = require 'data/scripts/path'
local Theme = require 'mods/dragons-lair/ui/theme'

local cornerWidth = 10
local cornerHeight = 10
local sideSize = 8

return {
    addedToMap = function(entity, widget)
        local container = Widget.makeFixedSize(1, 1)
        container:setBackgroundColor(Theme.DIALOG_COLOR)

        -- sides
        do
            local topWidget = Widget.makeImage(Path.getModFilePath 'ui/entity/dialog-top.png')
            topWidget:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP)
            topWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
            topWidget:setSize(1, sideSize)

            local bottomWidget = Widget.makeImage(Path.getModFilePath 'ui/entity/dialog-bottom.png')
            bottomWidget:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM)
            bottomWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
            bottomWidget:setSize(1, sideSize)

            local leftWidget = Widget.makeImage(Path.getModFilePath 'ui/entity/dialog-left.png')
            leftWidget:setPositionPolicy(Widget.PositionPolicy.LEFT + Widget.PositionPolicy.CENTER_Y)
            leftWidget:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
            leftWidget:setSize(sideSize, 1)

            local rightWidget = Widget.makeImage(Path.getModFilePath 'ui/entity/dialog-right.png')
            rightWidget:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.CENTER_Y)
            rightWidget:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
            rightWidget:setSize(sideSize, 1)

            container:addChild(topWidget)
            container:addChild(bottomWidget)
            container:addChild(leftWidget)
            container:addChild(rightWidget)
        end

        -- corners
        do
            local topLeftWidget = Widget.makeImage(Path.getModFilePath 'ui/entity/dialog-topleft.png')
            topLeftWidget:setPositionPolicy(Widget.PositionPolicy.LEFT + Widget.PositionPolicy.TOP)
            topLeftWidget:setSize(cornerWidth, cornerHeight)

            local bottomLeftWidget = Widget.makeImage(Path.getModFilePath 'ui/entity/dialog-bottomleft.png')
            bottomLeftWidget:setPositionPolicy(Widget.PositionPolicy.LEFT + Widget.PositionPolicy.BOTTOM)
            bottomLeftWidget:setSize(cornerWidth, cornerHeight)

            local topRightWidget = Widget.makeImage(Path.getModFilePath 'ui/entity/dialog-topRight.png')
            topRightWidget:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.TOP)
            topRightWidget:setSize(cornerWidth, cornerHeight)


            local bottomRightWidget = Widget.makeImage(Path.getModFilePath 'ui/entity/dialog-bottomRight.png')
            bottomRightWidget:setPositionPolicy(Widget.PositionPolicy.RIGHT + Widget.PositionPolicy.BOTTOM)
            bottomRightWidget:setSize(cornerWidth, cornerHeight)

            container:addChild(topLeftWidget)
            container:addChild(bottomLeftWidget)
            container:addChild(topRightWidget)
            container:addChild(bottomRightWidget)
        end

        widget:addChild(container)
        Widget.getRoot():addChild(widget)

        entity:setUiVisible(false)
        return container
    end,

    removedFromMap = function(entity, widget)
        widget:removeFromParent()
    end
}