local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ButtonPressedNode = FunctionalScriptNode:inherit 'Button Pressed'

function ButtonPressedNode:buildPins()
    self.buttonInPin = self:addInputPin(flat.types.STRING, 'Button')

    self.pressedOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Pressed')
end

function ButtonPressedNode:execute(runtime)
    local button = runtime:readPin(self.buttonInPin)

    local buttonId = assert(Gamepads.GamepadButton[button])
    local pressed = Gamepads.isPressed(0, buttonId)

    runtime:writePin(self.pressedOutPin, pressed)
end

return ButtonPressedNode