local UiSettings = require 'data/scripts/ui/uisettings'

local Radiobutton = {}
Radiobutton.__index = Radiobutton

local COLORS = {
    default = 0x0E0E0EFF,
    disabled = 0x838383FF,
}

local STATEPOSITION = {
    default = {{0,0}, {0,0.5}},
    disabled =  {{0.5,0}, {0.5,0.5}},
}
function Radiobutton:new(value, text, state)
    text  = text or ""
    value = value or text
    color = color or "blue"
    path  = 'data/editor/interface/controls/'
    state = state or "default"
    local checkMarkContainer = Widget.makeFixedSize(20,20)
    local checkMark = Widget.makeFixedSize(20,20)
    checkMark:setPosition(0, 0)
    checkMark:setPositionPolicy(Widget.PositionPolicy.CENTER)
    checkMark:setVisible(false)
    checkMarkContainer:addChild(checkMark)
    local container = Widget.makeLineFlow()
    container:setSizePolicy(Widget.SizePolicy.COMPRESS)
    container:addChild(checkMarkContainer)

    local label = Widget.makeText(text, table.unpack(UiSettings.defaultFont))
    label:setMargin(0, 5, 1, 5)

    container:addChild(label)

    local o = setmetatable({
        container             = container,
        checkMark             = checkMark,
        checkMarkContainer    = checkMarkContainer,
        label                 = label,
        value                 = value,
        text                  = text,
        state                 = state,
        path                  = path,
        valueChangedCallbacks = {}
    }, self)
    o:setStateColor()

    return o
end


function Radiobutton:setStateColor()
    self.checkMarkContainer:setBackground(self.path .. '/radiobutton.png')
    self.checkMarkContainer:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    self.checkMarkContainer:setBackgroundPosition(STATEPOSITION[self.state][1][1], STATEPOSITION[self.state][1][2])
    self.checkMark:setBackground(self.path .. '/radiobutton.png')
    self.checkMark:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    self.checkMark:setBackgroundPosition(STATEPOSITION[self.state][2][1], STATEPOSITION[self.state][2][2])
    self.label:setTextColor(COLORS[self.state])
end

function Radiobutton:getValue()
    return self.value
end
function Radiobutton:getState()
    return self.state
end

function Radiobutton:setState(state)
    self.state = state
    self:setStateColor()
end
function Radiobutton:toggleState()
    self.state = self.state == "default" and "disabled" or "default"
    self:setStateColor()
end
function Radiobutton:getText()
    return self.text
end

function Radiobutton:setVisible(visible)
    self.checkMark:setVisible(visible)
end

return Radiobutton