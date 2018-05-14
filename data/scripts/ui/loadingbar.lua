
local LoadingBar = {}
LoadingBar.__index = LoadingBar


local COLORS = {
    white      = 0xFFFFFFFF,
    black      = 0x000000FF,
    lightBlack = 0x1A1A1AFF,
    red        = 0xA72C2CFF,
    lightRed   = 0xEA6565FF,
    orange     = 0xC1732DFF,
    blue       = 0x133EC8FF,
    lightBlue  = 0x7C81E5FF,
    darkBlue   = 0x232896FF,
    cyan       = 0x37BFB7FF,
    green      = 0x2AA920FF,
    lightGreen = 0x7EB86CFF,
    darkGreen  = 0x357B3AFF,
    yellow     = 0xC1AC36FF,
    grey       = 0x626262FF,
    darkGrey   = 0x3D3D3DFF,
    lightGrey  = 0x545454FF,
}

function LoadingBar:new(width, height, value, color)
    value     = value or 0
    width     = width or 100
    height    = height or 10
    color     = color or "red"

    if type(color) == "string" then
        color = COLORS[color]
    end

    local padding   = 0
    local container = Widget.makeFixedSize(width, height)
    local content   = Widget.makeFixedSize(width * value / 100 - padding * 2, height - padding * 2)
    content:setBackgroundColor(color)
    container:setBackgroundColor(COLORS.lightGrey)
    container:setPadding(padding)
    container:addChild(content)


    local o = setmetatable({
        container             = container,
        content               = content,
        value                 = value,
        padding               = padding,
        color                 = color,
        timer                 = nil,
        callbacks             = {}
    }, self)


    return o
end

function LoadingBar:setValue(value)
    self.value = value
    if self.timer then
        self.timer:stop()
    end
    local w,h = self.container:getSize()
    local wc,hc = self.content:getSize()
    local wmax = w - self.padding * 2
    local maxTimer = 0.25
    self.timer = flat.Timer()
    self.timer:onUpdate(function(timer, elapsedTime)
        self.content:setSize(
            (wc + (1- (maxTimer - elapsedTime) / maxTimer) * ((wmax  * self.value / 100  - wc) )),
            hc
         )
    end)
    self.timer:start(maxTimer)
end



return LoadingBar