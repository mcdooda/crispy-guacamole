if not flat.debug then
    return
end

setmetatable(
    _ENV,
    {
        __index = function(t, name)
            error(debug.traceback('Trying to access a global variable \'' .. tostring(name) .. '\' that does not exist', 2))
        end,
        __newindex = function(t, name, value)
            error(debug.traceback('Trying to declare a global variable \'' .. tostring(name) .. '\' to \'' .. tostring(value) .. '\' ', 2))
        end
    }
)

function flat.debug.monitorAccesses(t, read, write)
    local mt = {}
    local values = {}
    if read then
        mt.__index = function(t, k)
            print '=============='
            print('Reading ' .. tostring(t) .. ': ' ..tostring(k) .. ' (' .. values[k] .. ')')
            flat.debug.printstack()
            print '=============='
            return values[k]
        end
    end
    if write then
        mt.__nweindex = function(t, k, v)
            print '=============='
            print('Writing ' .. tostring(t) .. ': ' ..tostring(k) .. ' = ' .. tostring(v) .. ')')
            flat.debug.printstack()
            print '=============='
            values[k] = v
        end
    end
    setmetatable(t, mt)
end

local UiSettings = require 'data/scripts/ui/uisettings'

local root = Widget.getRoot()
local debugContainer = Widget.makeColumnFlow()
debugContainer:setBackgroundColor(0xFFFFFF40)
debugContainer:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
debugContainer:setPadding(5, 5, 0, 5)

local hideDebugButton = Widget.makeText('<Hide>', table.unpack(UiSettings.defaultFont))
hideDebugButton:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
hideDebugButton:setTextColor(0x000000FF)
hideDebugButton:click(function()
    debugContainer:removeFromParent()
end)
debugContainer:addChild(hideDebugButton)

local function addContainer(title)
    local widget = Widget.makeColumnFlow()
    widget:setMargin(0, 0, 5, 0)
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
    local widget = Widget.makeFixedSize(1, 4)
    return widget
end

local debugFiles = {
    'stats',
    'drawstats',
    'profiler',
    'entitydebug',
    'pathfinding',
    'fog',
    'memorysnapshot',
    'gamespeed'
}

for i = 1, #debugFiles do
    dofile('data/scripts/' .. debugFiles[i] .. '.lua')(addContainer, makeSeparator, UiSettings.defaultFont)
end

root:addChild(debugContainer)