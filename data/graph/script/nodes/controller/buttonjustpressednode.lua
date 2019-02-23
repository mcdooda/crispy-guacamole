local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ButtonJustPressedNode = ScriptNode:inherit 'Button Just Pressed'

function ButtonJustPressedNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.buttonInPin = self:addInputPin(flat.types.STRING, 'Button')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.justPressedOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Just Pressed')
end

function ButtonJustPressedNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local button = runtime:readPin(self.buttonInPin)

    local buttonId = assert(Gamepads.GamepadButton[button])
    local justPressed = Gamepads.isJustPressed(0, buttonId)

    runtime:writePin(self.justPressedOutPin, justPressed)

    runtime:impulse(self.impulseOutPin)
end

return ButtonJustPressedNode