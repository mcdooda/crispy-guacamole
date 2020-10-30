local function addProgressBarToMap(entity, widget, offsetY, bgpicto, fgpicto, widgetWidth, widgetHeight)
    local extraData = entity:getExtraData()
    extraData.progressChanged = flat.Slot:new()

    local innerWidgetMaxWidth = widgetWidth - 2
    local innerWidgetHeight = widgetHeight - 2

    local progressBar = Widget.makeFixedSize(widgetWidth, widgetHeight)
    progressBar:setBackground(bgpicto)
    local progressBarInnerWidget = Widget.makeFixedSize(innerWidgetMaxWidth, innerWidgetHeight)
    progressBarInnerWidget:setBackground(fgpicto)
    progressBarInnerWidget:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    progressBarInnerWidget:setPosition(1, -1)
    progressBar:setMargin(1,0,0,0)
    progressBar:addChild(progressBarInnerWidget)

    widget:addChild(progressBar)

    progressBar:hide()
    entity:setUiOffset(flat.Vector2(-widgetWidth / 2, offsetY))

    extraData.progressChanged:on(function(progress)
        if progress ~= 0 then
            progressBar:show()
            local innerWidgetWidth = progress * innerWidgetMaxWidth
            progressBarInnerWidget:setSize(innerWidgetWidth, innerWidgetHeight)
        else
            progressBar:hide()
        end
    end)
end


return {
	addProgressBarToMap = addProgressBarToMap
}