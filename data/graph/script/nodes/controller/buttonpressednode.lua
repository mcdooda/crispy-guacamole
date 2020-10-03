local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ButtonPressedNode = FunctionalScriptNode:inherit 'Button Pressed'

function ButtonPressedNode:buildPins()
    self.buttonInPin = self:addInputPin(flat.types.STRING, 'Button')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.pressedOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Pressed')
end

function ButtonPressedNode:execute(runtime)
    local button = runtime:readPin(self.buttonInPin)
    local entity = runtime:readPin(self.entityInPin)
    local gamepadIndex = entity:getGamepadIndex()

    local buttonId = assert(Gamepads.GamepadButton[button])
    local pressed = Gamepads.isPressed(gamepadIndex, buttonId)

    runtime:writePin(self.pressedOutPin, pressed)
end

return ButtonPressedNode