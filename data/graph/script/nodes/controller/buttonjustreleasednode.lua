local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ButtonJustReleasedNode = FunctionalScriptNode:inherit 'Button Just Released'

function ButtonJustReleasedNode:buildPins()
    self.buttonInPin = self:addInputPin(flat.types.STRING, 'Button')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.justReleasedOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Just Released')
end

function ButtonJustReleasedNode:execute(runtime)
    local button = runtime:readPin(self.buttonInPin)
    local entity = runtime:readPin(self.entityInPin)
    local gamepadIndex = entity:getGamepadIndex()

    local buttonId = assert(Gamepads.GamepadButton[button])
    local justReleased = Gamepads.isJustReleased(gamepadIndex, buttonId)

    runtime:writePin(self.justReleasedOutPin, justReleased)
end

return ButtonJustReleasedNode