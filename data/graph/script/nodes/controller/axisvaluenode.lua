local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local AxisValueNode = FunctionalScriptNode:inherit 'Axis Value'

function AxisValueNode:buildPins()
    self.axisInPin = self:addInputPin(flat.types.STRING, 'Axis')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.axisValueOutPin = self:addOutputPin(flat.types.NUMBER, 'Axis Value')
end

function AxisValueNode:execute(runtime)
    local axis = runtime:readPin(self.axisInPin)
    local entity = runtime:readPin(self.entityInPin)
    local gamepadIndex = entity:getGamepadIndex()

    local axisId = assert(Gamepads.GamepadAxis[axis])
    local axisValue = Gamepads.getAxisValue(gamepadIndex, axisId)

    runtime:writePin(self.axisValueOutPin, axisValue)
end

return AxisValueNode