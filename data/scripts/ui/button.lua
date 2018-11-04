local Button = {}
Button.__index = Button



local COLORS = {
    white      = 0xFFFFFFFF,
    black      = 0x000000FF,
    lightBlack = 0x1A1A1AFF,
    red        = 0xA72C2CFF,
    lightRed   = 0xEA6565FF,
    orange     = 0xC1732DFF,
    blue       = 0x133EC8FF,
    lightBlue  = 0x7C81E5FF,
    clearBlue  = 0xC7E5FAFF,
    darkBlue   = 0x232896FF,
    cyan       = 0x37BFB7FF,
    green      = 0x2AA920FF,
    lightGreen = 0x7EB86CFF,
    darkGreen  = 0x357B3AFF,
    yellow     = 0xC1AC36FF,
    grey       = 0x626262FF,
    darkGrey   = 0x3D3D3DFF,
    lightGrey  = 0x545454FF,
    transparent= 0x00000000
}


local function triggerClick(button)
    local callbacks = button.callbacks
    for i = 1, #callbacks do
        callbacks[i](button)
    end
end

local function mouseUpContainer(button)
    button.container:setBackgroundColor(button.color)
    return true
end

local function mouseDownContainer(button)
    button.container:setBackgroundColor(button.clickColor)
    triggerClick(button)
    return true
end
local function mouseEnter(button)
    button.container:setBackgroundColor(button.hoverColor)
    return true
end
local function mouseLeave(button)
    button.container:setBackgroundColor(button.color)
    return true
end
function Button:click(callback)
    self.container:click(callback)
    return true
end

function Button:new(content, options, callbacks)
    content    = content or Widget.makeFixedSize(10,10)
    options    = options or {color="grey"}
    local margin     = options.margin or 5
    local padding    = options.padding or {8, 20, 8, 20}
    local color      = options.color or "grey"
    local clickColor = options.clickColor or "lightBlack"
    local hoverColor = options.hoverColor or "lightGrey"

    if type(hoverColor) == "string" then
        hoverColor = COLORS[hoverColor]
    end

    if type(color) == "string" then
        color = COLORS[color]
    end

    if type(clickColor) == "string" then
        clickColor = COLORS[clickColor]
    end

    callbacks = callbacks or {}

    local container = Widget.makeCompress()
    container:addChild(content)
    if type(padding) == "table" then
        container:setPadding(padding[1], padding[2], padding[3], padding[4])
    elseif (padding ~= nil) then
        container:setPadding(padding)
    end
    container:setBackgroundColor(color)
    container:setMargin(margin)

    local o = setmetatable({
        container     = container,
        content       = content,
        hoverColor    = hoverColor,
        clickColor    = clickColor,
        color         = color,
        callbacks     = {}
    }, self)

    -- local click = function(c) containerClicked(o) end
    -- container:click(click)
    container:mouseEnter(function(c) mouseEnter(o) end)
    container:mouseLeave(function(c) mouseLeave(o) end)
    container:mouseUp(function(c) mouseUpContainer(o) end)
    container:mouseDown(function(c) mouseDownContainer(o) end)

    return o
end



return Button