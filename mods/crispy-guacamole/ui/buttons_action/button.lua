local Path  = require 'data/scripts/path'
local Button = {}
Button.__index = Button

local buttonRadius = 40

function Button:new(parent, type, state)
    local o = setmetatable({
        container = nil,
        type = type,
        state = state
    }, self)
    o:build(parent)
    return o
end

function Button:setPressed(pressed)
    if(pressed) then
        self.container:setBackground(Path.getModFilePath('ui/controller/xbox/xbox_' .. self.type .. '_pressed.png'))
    else
        self.container:setBackground(Path.getModFilePath('ui/controller/xbox/xbox_' .. self.type .. '.png'))
    end
end

function Button:build(parent)
    self.container = Widget.makeFixedSize(buttonRadius, buttonRadius)
    self:setPressed(self.state)
    parent:addChild(self.container)
    return self
end

return Button
