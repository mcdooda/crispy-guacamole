local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ButtonJustReleasedNode = FunctionalScriptNode:inherit 'Button Just Released'

function ButtonJustReleasedNode:buildPins()
    self.buttonInPin = self:addInputPin(flat.types.STRING, 'Button')

    self.justReleasedOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Just Released')
end

function ButtonJustReleasedNode:execute(runtime)
    local button = runtime:readPin(self.buttonInPin)

    local buttonId = assert(Gamepads.GamepadButton[button])
    local justReleased = Gamepads.isJustReleased(0, buttonId)

    runtime:writePin(self.justReleasedOutPin, justReleased)
end

return ButtonJustReleasedNode