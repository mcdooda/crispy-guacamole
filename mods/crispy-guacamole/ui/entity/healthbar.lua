local function addHealthBarToMap(entity, widget, offsetY, bgpicto, fgpicto, widgetWidth, widgetHeight)
    local innerWidgetMaxWidth = widgetWidth - 2
    local innerWidgetHeight = widgetHeight - 2
    local healthBarWidget = Widget.makeFixedSize(widgetWidth, widgetHeight)
    healthBarWidget:setBackground(bgpicto)

    local healthBarInnerWidget = Widget.makeFixedSize(widgetWidth - 2, widgetHeight - 2)
    healthBarInnerWidget:setBackground(fgpicto)
    healthBarInnerWidget:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    healthBarInnerWidget:setPosition(1, -1)
    healthBarWidget:addChild(healthBarInnerWidget)

    widget:addChild(healthBarWidget)

    Widget.getRoot():addChild(widget)

    healthBarWidget:hide()
    entity:setUiOffset(flat.Vector2(-widgetWidth / 2, offsetY))
    entity:healthChanged(function(previousHealth, currentHealth, maxHealth)
        local healthRatio = currentHealth / maxHealth
        if 0 < healthRatio and healthRatio < 1 then
            local innerWidgetWidth = healthRatio * innerWidgetMaxWidth
            healthBarWidget:show()
            healthBarInnerWidget:setSize(innerWidgetWidth, innerWidgetHeight)
        else
            healthBarWidget:hide()
        end
    end)
end

return {
	addHealthBarToMap = addHealthBarToMap
}