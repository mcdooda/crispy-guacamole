local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local AxisValueNode = FunctionalScriptNode:inherit 'Axis Value'

function AxisValueNode:buildPins()
    self.axisInPin = self:addInputPin(flat.types.STRING, 'Axis')

    self.axisValueOutPin = self:addOutputPin(flat.types.NUMBER, 'Axis Value')
end

function AxisValueNode:execute(runtime)
    local axis = runtime:readPin(self.axisInPin)

    local axisId = assert(Gamepads.GamepadAxis[axis])
    local axisValue = Gamepads.getAxisValue(0, axisId)

    runtime:writePin(self.axisValueOutPin, axisValue)
end

return AxisValueNode