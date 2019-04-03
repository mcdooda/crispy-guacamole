local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ButtonJustPressedNode = FunctionalScriptNode:inherit 'Button Just Pressed'

function ButtonJustPressedNode:buildPins()
    self.buttonInPin = self:addInputPin(flat.types.STRING, 'Button')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.justPressedOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Just Pressed')
end

function ButtonJustPressedNode:execute(runtime)
    local button = runtime:readPin(self.buttonInPin)
    local entity = runtime:readPin(self.entityInPin)
    local gamepadIndex = entity:getGamepadIndex()

    local buttonId = assert(Gamepads.GamepadButton[button])
    local justPressed = Gamepads.isJustPressed(gamepadIndex, buttonId)

    runtime:writePin(self.justPressedOutPin, justPressed)
end

return ButtonJustPressedNode