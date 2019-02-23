local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ButtonPressedNode = ScriptNode:inherit 'Button Pressed'

function ButtonPressedNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.buttonInPin = self:addInputPin(flat.types.STRING, 'Button')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.pressedOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Pressed')
end

function ButtonPressedNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local button = runtime:readPin(self.buttonInPin)

    local buttonId = assert(Gamepads.GamepadButton[button])
    local pressed = Gamepads.isPressed(0, buttonId)

    runtime:writePin(self.pressedOutPin, pressed)

    runtime:impulse(self.impulseOutPin)
end

return ButtonPressedNode