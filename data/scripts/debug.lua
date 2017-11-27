if not flat.debug then
    return
end

local UiSettings = require 'data/scripts/ui/uisettings'

local root = Widget.getRoot()
local debugContainer = Widget.makeColumnFlow()
debugContainer:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)

local function addContainer(title)
    local widget = Widget.makeColumnFlow()
    widget:setMargin(5, 5, 0, 5)
    widget:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
    widget:setBackgroundColor(0xFFFFFFFF)
    
    local container = Widget.makeColumnFlow()
    container:setPadding(2)
    local visible = false

    local collapseButtonContainer = Widget.makeFixedSize(120,1)
    collapseButtonContainer:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.COMPRESS_Y)
    do
        local collapseButton = Widget.makeText(title, table.unpack(UiSettings.defaultFont))
        collapseButton:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP)
        collapseButton:setTextColor(0x000000FF)
        collapseButtonContainer:addChild(collapseButton)
    end
    collapseButtonContainer:click(function()
        if visible then
            widget:removeChild(container)
        else
            widget:addChild(container)
        end
        visible = not visible
    end)
    widget:addChild(collapseButtonContainer)

    debugContainer:addChild(widget)
    return container
end

local function makeSeparator()
    local widget = Widget.makeFixedSize(4, 1)
    return widget
end

dofile 'data/scripts/stats.lua' (addContainer, makeSeparator, UiSettings.defaultFont)
dofile 'data/scripts/entitydebug.lua' (addContainer, makeSeparator, UiSettings.defaultFont)
dofile 'data/scripts/memorysnapshot.lua' (addContainer, makeSeparator, UiSettings.defaultFont)
dofile 'data/scripts/gamespeed.lua' (addContainer, makeSeparator, UiSettings.defaultFont)

root:addChild(debugContainer)