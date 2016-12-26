
local font = { 'data/misc/fonts/Lato-Medium.ttf', 14 }

local Button = {}
Button.__index = Button



local function triggerClick(button)
    local callbacks = button.callbacks
    for i = 1, #callbacks do
        callbacks[i](button, value)
    end
end

local function mouseUpContainer(button)
    button.padding:setSize(0, 3)
    return true
end

local function containerClicked(button)
    triggerClick(button, value)
    return true
end

local function mouseDownContainer(button)
    button.padding:setSize(0, 1)
    return true
end
local function mouseEnter(button)
    button.container:setMargin(6,5,5,5)
    button.padding:setSize(0, 2)
    return true
end
local function mouseLeave(button)
    button.container:setMargin(5)
    button.padding:setSize(0, 3)
    return true
end

function Button:new(content, color, callbacks)
    content    = content or Widget.makeFixedSize(10,10)
    color      = color or "grey"

    if type(color) == "string" then
        color = COLORS[color]
    end

    callbacks  = callbacks or {}

    local container1 = Widget.makeLineFlow()
    container1:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.COMPRESS_Y)
    container1:addChild(content)
    container1:setPadding(8,20,8,20)
    local container = Widget.makeColumnFlow()
    local padding = Widget.makeColumnFlow()
    padding:setSize(0,3)
    padding:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    padding:setBackgroundColor(0xFFFFFF99)
    container:setBackgroundColor(color)
    container:setMargin(5)

    container:addChild(container1)
    container:addChild(padding)
    local o = setmetatable({
        container     = container,
        padding       = padding,
        content       = content,
        callbacks     = {}
    }, self)

    local click = function(c) containerClicked(o) end
    -- container:click(click)
    container:click(function(c) mouseEnter(o) end)
    container:mouseLeave(function(c) mouseLeave(o) end)
    -- container:mouseUp(function(c) mouseUpContainer(o) end)
    -- container:mouseDown(function(c) mouseDownContainer(o) end)

    return o
end



return Button