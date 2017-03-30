local root = Widget.getRoot()
local font = { 'data/misc/fonts/LucidaSansRegular.ttf', 12 }
local debugContainer = Widget.makeColumnFlow()
debugContainer:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)

local function addContainer()
    local widget = Widget.makeColumnFlow()
	widget:setMargin(5, 5, 0, 5)
    widget:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
	widget:setBackgroundColor(0xFFFFFFAA)
    debugContainer:addChild(widget)
    return widget
end

local function makeSeparator()
    local widget = Widget.makeFixedSize(4, 1)
    return widget
end

dofile 'data/scripts/stats.lua' (addContainer, makeSeparator, font)
dofile 'data/scripts/entitydebug.lua' (addContainer, makeSeparator, font)

root:addChild(debugContainer)