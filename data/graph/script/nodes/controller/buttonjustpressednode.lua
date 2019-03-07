local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ButtonJustPressedNode = FunctionalScriptNode:inherit 'Button Just Pressed'

function ButtonJustPressedNode:buildPins()
    self.buttonInPin = self:addInputPin(flat.types.STRING, 'Button')

    self.justPressedOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Just Pressed')
end

function ButtonJustPressedNode:execute(runtime)
    local button = runtime:readPin(self.buttonInPin)

    local buttonId = assert(Gamepads.GamepadButton[button])
    local justPressed = Gamepads.isJustPressed(0, buttonId)

    runtime:writePin(self.justPressedOutPin, justPressed)
end

return ButtonJustPressedNode