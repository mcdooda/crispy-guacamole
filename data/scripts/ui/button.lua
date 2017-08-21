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
    if button.bezel then
        button.container:setMargin(6,5,5,5)
        button.padding:setSize(0, 1)
    end
    triggerClick(button, value)
    return true
end

local function mouseDownContainer(button)
    if button.bezel then
        button.padding:setSize(0, 1)
    end
    return true
end
local function mouseEnter(button)
    if button.hoverColor ~= nil then
        button.container:setBackgroundColor(button.hoverColor)
    end
    return true
end
local function mouseLeave(button)
    button.container:setBackgroundColor(button.color)
    if button.bezel then
        button.container:setMargin(5)
        button.padding:setSize(0, 3)
    end
    return true
end

function Button:new(content, options, callbacks)
    content    = content or Widget.makeFixedSize(10,10)
    options    = options or {color="grey", bezel=true}
    local margin     = options.margin or 5
    local padding    = options.padding or {t=8, r=20, b=8 , l=20}
    local color      = options.color or "grey"
    local bezel      = options.bezel
    local hoverColor = options.hoverColor

    if type(hoverColor) == "string" then
        hoverColor = COLORS[hoverColor]
    end

    if type(color) == "string" then
        color = COLORS[color]
    end

    callbacks  = callbacks or {}

    local container1 = Widget.makeLineFlow()
    container1:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.COMPRESS_Y)
    container1:addChild(content)
    container1:setPadding(padding.t, padding.r, padding.b, padding.l)
    local container = Widget.makeColumnFlow()
    local padding = Widget.makeColumnFlow()
    padding:setSize(0, 3)
    padding:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    padding:setBackgroundColor(0xFFFFFF99)
    container:setBackgroundColor(color)
    container:setMargin(margin)

    container:addChild(container1)
    if bezel then
        container:addChild(padding)
    end
    local o = setmetatable({
        container     = container,
        padding       = padding,
        content       = content,
        bezel         = bezel,
        hoverColor    = hoverColor,
        color         = color,
        callbacks     = {}
    }, self)

    local click = function(c) containerClicked(o) end
    container:click(click)
    container:mouseEnter(function(c)
        print("INSIDE")
        mouseEnter(o)
    end)
    container:mouseLeave(function(c) mouseLeave(o) end)
    -- container:mouseUp(function(c) mouseUpContainer(o) end)
    -- container:mouseDown(function(c) mouseDownContainer(o) end)

    return o
end



return Button