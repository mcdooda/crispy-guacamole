
local font = { 'data/misc/fonts/Lato-Medium.ttf', 12 }

local Checkbox = {}
Checkbox.__index = Checkbox

local COLORS = {
    default  = 0x0E0E0EFF,
    disabled = 0x838383FF,
}

local STATEPOSITION = {
    default  = {{0,0}, {0,0.5}},
    disabled =  {{0.5,0}, {0.5,0.5}},
}

local function clicked(checkBox)
    if checkBox:getState() ~= 'disabled' then
        checkBox:setValue(not checkBox:getValue())
    end
    return true
end

function Checkbox:new(value, text, state)
    text  = text or ""
    value = value or false
    path  = 'data/editor/interface/controls/'
    state = state or "default"
    local checkMarkContainer = Widget.makeFixedSize(20,20)
    local checkMark = Widget.makeFixedSize(20,20)
    checkMark:setPosition(0, 0)
    checkMark:setPositionPolicy(Widget.PositionPolicy.CENTER)
    checkMark:setVisible(value)
    checkMarkContainer:addChild(checkMark)
    local container = Widget.makeLineFlow()
    container:setSizePolicy(Widget.SizePolicy.COMPRESS)
    container:addChild(checkMarkContainer)


    local label = Widget.makeText(text, unpack(font))
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
    container:click(function(c) clicked(o) end)

    return o
end


function Checkbox:setStateColor()
    self.checkMarkContainer:setBackground(self.path .. '/checkbox.png')
    self.checkMarkContainer:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    self.checkMarkContainer:setBackgroundPosition(STATEPOSITION[self.state][1][1], STATEPOSITION[self.state][1][2])
    self.checkMark:setBackground(self.path .. '/checkbox.png')
    self.checkMark:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    self.checkMark:setBackgroundPosition(STATEPOSITION[self.state][2][1], STATEPOSITION[self.state][2][2])
    self.label:setTextColor(COLORS[self.state])
end

function Checkbox:getValue()
    return self.value
end
function Checkbox:setValue(value)
    self.value = value
    self:setVisible(value)
end
function Checkbox:toggleValue()
    self:setValue(not self:getValue())
end
function Checkbox:getState()
    return self.state
end

function Checkbox:setState(state)
    self.state = state
    self:setStateColor()
end

function Checkbox:toggleState()
    self:setState(self.state == "default" and "disabled" or "default")
end

function Checkbox:getText()
    return self.text
end

function Checkbox:setVisible(visible)
    self.checkMark:setVisible(visible)
end

return Checkbox